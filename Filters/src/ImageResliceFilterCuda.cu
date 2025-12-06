#include "ImageResliceFilterCuda.cuh"
// #include <spdlog/spdlog.h>
#include <iostream>
#include <stdexcept>
#include <thrust/device_ptr.h>
#include <thrust/extrema.h>
// #define BENCHMARK

namespace FilterKernel
{
__device__ float4 multiply(const glm::mat4& mat, const float4& point)
{
  return make_float4(
    mat[0][0] * point.x + mat[1][0] * point.y + mat[2][0] * point.z + mat[3][0] * point.w,
    mat[0][1] * point.x + mat[1][1] * point.y + mat[2][1] * point.z + mat[3][1] * point.w,
    mat[0][2] * point.x + mat[1][2] * point.y + mat[2][2] * point.z + mat[3][2] * point.w,
    mat[0][3] * point.x + mat[1][3] * point.y + mat[2][3] * point.z + mat[3][3] * point.w);
}
template <typename ComponentType>
__global__ void resliceVolume(
  Volume* volume, cudaTextureObject_t texture, Plane* plane, ComponentType* pixels)
{
  // already know plane bounds world coordinate
  const unsigned int x = blockDim.x * blockIdx.x + threadIdx.x;
  const unsigned int y = blockDim.y * blockIdx.y + threadIdx.y;

  if (x >= plane->m_Width || y >= plane->m_Height)
  {
    return;
  }
  float4 worldPlanePixel = multiply(plane->m_IndexToWorld, make_float4(x, y, 0, 1.0f));

  float4 volumeIndex = multiply(volume->m_WorldToIndex, worldPlanePixel);

  auto data = tex3D<ComponentType>(texture, volumeIndex.x, volumeIndex.y, volumeIndex.z);
  pixels[y * plane->m_Width + x] = data;
}

template <typename pixelType>
struct NormalizeFunctor
{
  pixelType mMinValue;
  pixelType mRange;

  NormalizeFunctor(pixelType minValue, pixelType maxValue)
    : mMinValue(minValue)
    , mRange(maxValue - minValue)
  {
  }
  __host__ __device__ pixelType operator()(const pixelType& value)
  {
    if (mRange == 0)
      return static_cast<pixelType>(0);

    return (value - mMinValue) / mRange;
  }
};

}

ImageResliceFilterCuda::ImageResliceFilterCuda()
  : m_Texture(0)
  , m_dVolume(nullptr)
  , m_dPlane(nullptr)
  , m_hPixels(nullptr)
  , m_hVolume(nullptr)
{
}

ImageResliceFilterCuda::~ImageResliceFilterCuda()
{
  destroyResources();
}

void ImageResliceFilterCuda::setVolume(std::shared_ptr<Volume> volume)
{
  if (!m_hVolume || m_hVolume->m_Data != volume->m_Data)
  {
    m_hVolume = volume;
  }
}
Volume* ImageResliceFilterCuda::getVolume()
{
  return m_hVolume.get();
}

void ImageResliceFilterCuda::setPlane(std::shared_ptr<Plane> plane)
{
  m_hPlane = plane;
  if (m_hVolume)
  {
    uploadPlane(plane);
  }
}
Plane* ImageResliceFilterCuda::getPlane()
{
  return m_hPlane.get();
}

void ImageResliceFilterCuda::doFilter()
{
  if (m_dVolume == nullptr || m_hPlane == nullptr)
  {
    uploadVolume(m_hVolume);
    uploadPlane(m_hPlane);
  }

  launchResliceKernel();
}

void ImageResliceFilterCuda::uploadVolume(std::shared_ptr<Volume> volume)
{
  if (!volume)
  {
    /*SPDLOG_DEBUG(
      "volume is nullptr: {}, plane is nullptr: {}", volume == nullptr, plane == nullptr);*/
    return;
  }
  if (!volume->m_Data)
  {
    // SPDLOG_DEBUG("volume data is nullptr");
    return;
  }

  // upload volume
  auto err = cudaMalloc(&m_dVolume, sizeof(Volume));
  std::cout << __LINE__ << ": " << __FUNCTION__ << ":" << cudaGetErrorString(err) << std::endl;
  err = cudaMemcpy(m_dVolume, volume.get(), sizeof(Volume), cudaMemcpyHostToDevice);
  std::cout << __LINE__ << ": " << __FUNCTION__ << ":" << cudaGetErrorString(err) << std::endl;

  // upload volume data to texture
  cudaExtent extent =
    make_cudaExtent(volume->m_Dimensions.x, volume->m_Dimensions.y, volume->m_Dimensions.z);

  cudaArray_t dataArray{};
  cudaChannelFormatDesc channelFormat = createChannelFormat();
  cudaMalloc3DArray(&dataArray, &channelFormat, extent);
  cudaMemcpy3DParms params{};
  params.srcPtr = make_cudaPitchedPtr(m_hVolume->m_Data,
    m_hVolume->m_Dimensions.x * getDataTypeSize(volume->m_DataType), m_hVolume->m_Dimensions.x,
    m_hVolume->m_Dimensions.y);
  params.extent = extent;
  params.kind = cudaMemcpyHostToDevice;
  params.dstArray = dataArray;
  err = cudaMemcpy3D(&params);
  std::cout << __LINE__ << ": " << __FUNCTION__ << ":" << cudaGetErrorString(err) << std::endl;

  cudaResourceDesc desc{};
  desc.resType = cudaResourceTypeArray;
  desc.res.array.array = dataArray;
  cudaTextureDesc textureDesc{};
  textureDesc.addressMode[0] = cudaTextureAddressMode::cudaAddressModeClamp;
  textureDesc.addressMode[1] = cudaTextureAddressMode::cudaAddressModeClamp;
  textureDesc.addressMode[2] = cudaTextureAddressMode::cudaAddressModeClamp;
  textureDesc.borderColor[0] = 0.0f;
  textureDesc.borderColor[1] = 0.0f;
  textureDesc.borderColor[2] = 0.0f;
  textureDesc.borderColor[3] = 0.0f;
  textureDesc.filterMode = cudaTextureFilterMode::cudaFilterModePoint;
  textureDesc.readMode = cudaTextureReadMode::cudaReadModeElementType;
  textureDesc.normalizedCoords = 0;

  err = cudaCreateTextureObject(&m_Texture, &desc, &textureDesc, nullptr);
  std::cout << __LINE__ << ": " << __FUNCTION__ << ":" << cudaGetErrorString(err) << std::endl;

  m_hVolume = volume;
}
void ImageResliceFilterCuda::uploadPlane(std::shared_ptr<Plane> plane)
{
  // upload plane
  cudaError_t err;
  if (!m_dPlane)
  {
    err = cudaMalloc(&m_dPlane, sizeof(Plane));
    std::cout << __LINE__ << ": " << __FUNCTION__ << ":" << cudaGetErrorString(err) << std::endl;
  }

  err = cudaMemcpy(m_dPlane, plane.get(), sizeof(Plane), cudaMemcpyHostToDevice);
  std::cout << __LINE__ << ": " << __FUNCTION__ << ":" << cudaGetErrorString(err) << std::endl;

  // create cache for plane pixels;
  err = cudaMalloc(
    &m_Pixels, plane->m_Width * plane->m_Height * getDataTypeSize(m_hVolume->m_DataType));
  std::cout << __LINE__ << ": " << __FUNCTION__ << ":" << cudaGetErrorString(err) << std::endl;
  err = cudaMallocHost(
    &m_hPixels, plane->m_Width * plane->m_Height * getDataTypeSize(m_hVolume->m_DataType));
  std::cout << __LINE__ << ": " << __FUNCTION__ << ":" << cudaGetErrorString(err) << std::endl;
  memset(m_hPixels, 0, plane->m_Width * plane->m_Height * getDataTypeSize(m_hVolume->m_DataType));

  err = cudaMemset(
    m_Pixels, 0, plane->m_Width * plane->m_Height * getDataTypeSize(m_hVolume->m_DataType));
  std::cout << __LINE__ << ": " << __FUNCTION__ << ":" << cudaGetErrorString(err) << std::endl;

  m_hPlane = plane;
}

void ImageResliceFilterCuda::destroyResources()
{
  if (m_dVolume || m_dPlane)
  {
    auto err = cudaFree(m_dVolume);
    std::cout << __LINE__ << ": " << __FUNCTION__ << ":" << cudaGetErrorString(err) << std::endl;
    err = cudaFree(m_dPlane);
    std::cout << __LINE__ << ": " << __FUNCTION__ << ":" << cudaGetErrorString(err) << std::endl;
    err = cudaDestroyTextureObject(m_Texture);
    std::cout << __LINE__ << ": " << __FUNCTION__ << ":" << cudaGetErrorString(err) << std::endl;
    cudaFreeHost(m_hPixels);
  }
}
cudaChannelFormatDesc ImageResliceFilterCuda::createChannelFormat()
{
  switch (m_hVolume->m_DataType)
  {
    case DataType::CHAR:
      return cudaCreateChannelDesc<data_type<DataType::CHAR>>();
    case DataType::UNSIGNED_CHAR:
      return cudaCreateChannelDesc<data_type<DataType::UNSIGNED_CHAR>>();
    case DataType::SHORT:
      return cudaCreateChannelDesc<data_type<DataType::SHORT>>();
    case DataType::UNSIGNED_SHORT:
      return cudaCreateChannelDesc<data_type<DataType::UNSIGNED_SHORT>>();
    case DataType::FLOAT:
      return cudaCreateChannelDesc<data_type<DataType::FLOAT>>();
    default:
      throw std::runtime_error("unsupported type");
  }
}
void ImageResliceFilterCuda::launchResliceKernel()
{
  switch (m_hVolume->m_DataType)
  {
    case DataType::CHAR:
      return launchResliceKernelImpl<DataType::CHAR>();
    case DataType::UNSIGNED_CHAR:
      return launchResliceKernelImpl<DataType::UNSIGNED_CHAR>();
    case DataType::SHORT:
      return launchResliceKernelImpl<DataType::SHORT>();
    case DataType::UNSIGNED_SHORT:
      return launchResliceKernelImpl<DataType::UNSIGNED_SHORT>();
    case DataType::FLOAT:
      return launchResliceKernelImpl<DataType::FLOAT>();
    default:
      throw std::runtime_error("unsupported type");
  }
}

const void* ImageResliceFilterCuda::getPixels() const
{
  return m_hPixels;
}

template <DataType dt>
void ImageResliceFilterCuda::launchResliceKernelImpl()
{
#if defined(BENCHMARK)
  cudaEvent_t startEvent, stopEvent;
  cudaEventCreate(&startEvent);
  cudaEventCreate(&stopEvent);
  cudaEventRecord(startEvent);
#endif
  using type = data_type<dt>;
  dim3 blockSize(32, 32, 1);
  dim3 gridSize((m_hPlane->m_Width + blockSize.x - 1) / blockSize.x,
    (m_hPlane->m_Height + blockSize.y - 1) / blockSize.y, 1);
#if defined(BENCHMARK)
  const int count = 100;
  for (int i = 0; i < count; ++i)
  {
#endif
    FilterKernel::resliceVolume<type>
      <<<gridSize, blockSize>>>(m_dVolume, m_Texture, m_dPlane, static_cast<type*>(m_Pixels));

    thrust::device_ptr<type> pixelDataPtr(static_cast<type*>(m_Pixels));
    auto [minValue, maxValue] =
      thrust::minmax_element(pixelDataPtr, pixelDataPtr + m_hPlane->m_Width * m_hPlane->m_Height);
    thrust::transform(pixelDataPtr, pixelDataPtr + m_hPlane->m_Width * m_hPlane->m_Height,
      pixelDataPtr, FilterKernel::NormalizeFunctor<type>(*minValue, *maxValue));

#if defined(BENCHMARK)
  }
#endif
#if defined(BENCHMARK)
  cudaEventRecord(stopEvent);
#endif
  cudaDeviceSynchronize();
#if defined(BENCHMARK)
  float elapsedTime = 0.0f;
  cudaEventElapsedTime(&elapsedTime, startEvent, stopEvent);
  std::cout << "slice time: " << elapsedTime / count << "ms" << std::endl;
#endif
  cudaError_t err = cudaGetLastError();
  std::cout << __LINE__ << ": " << __FUNCTION__ << ":" << cudaGetErrorString(err) << std::endl;
  if (err != cudaSuccess)
  {
    std::cout << "params: " << std::endl
              << "grid size: " << gridSize.x << " " << gridSize.y << " " << gridSize.z << std::endl
              << "type: " << typeid(type).name() << std::endl
              << "m_dVolume address: " << m_dVolume << std::endl
              << "m_Texture id: " << m_Texture << std::endl
              << "m_dPlane address: " << m_dPlane << std::endl
              << "m_Pixels address: " << m_Pixels << ": " << static_cast<type*>(m_Pixels)
              << std::endl;
  }
  else
  {
    cudaMemcpy(m_hPixels, m_Pixels,
      m_hPlane->m_Width * m_hPlane->m_Height * getDataTypeSize(m_hVolume->m_DataType),
      cudaMemcpyDeviceToHost);
  }
}
