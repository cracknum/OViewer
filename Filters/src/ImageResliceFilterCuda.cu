#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#ifndef WINGDIAPI
#define WINGDIAPI __declspec(dllimport)
#endif

#ifndef APIENTRY
#define APIENTRY __stdcall
#endif

#include "ImageResliceFilterCuda.cuh"
#define FMT_UNICODE 0
#include <cuda_gl_interop.h>
#include <iostream>
#include <spdlog/spdlog.h>
#include <sstream>
#include <stdexcept>
// #define BENCHMARK
#define LOG_ERROR

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
__global__ void resliceVolume(Volume* volume, cudaTextureObject_t texture, Plane* plane,
  cudaSurfaceObject_t textureSurface, float2 windowLevel)
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

  surf2Dwrite(data, textureSurface, x * sizeof(ComponentType), y);
}
}

ImageResliceFilterCuda::ImageResliceFilterCuda()
  : m_Texture(0)
  , m_dVolume(nullptr)
  , m_dPlane(nullptr)
  , m_hVolume(nullptr)
  , m_GLTexture(0)
  , m_Resource(nullptr)
  , m_WindowLevel(make_float2(0, 0))
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

void ImageResliceFilterCuda::setGLTexture(unsigned int glTexture)
{
  m_GLTexture = glTexture;
  if (m_Resource)
  {
    cudaGraphicsUnregisterResource(m_Resource);
  }

  cudaGraphicsGLRegisterImage(
    &m_Resource, glTexture, GL_TEXTURE_2D, cudaGraphicsRegisterFlagsWriteDiscard);
}

void ImageResliceFilterCuda::doFilter()
{
  if (!m_Resource)
  {
    SPDLOG_ERROR("no texture registered into cuda");
    return;
  }
  if (m_WindowLevel.x < 1e-6 || m_WindowLevel.y < 1e-6)
  {
    SPDLOG_ERROR("window level is not set");
    return;
  }

  if (m_dVolume == nullptr || m_hPlane == nullptr)
  {
    uploadVolume(m_hVolume);
    uploadPlane(m_hPlane);
  }
  cudaGraphicsMapResources(1, &m_Resource);
  cudaArray_t array;
  cudaGraphicsSubResourceGetMappedArray(&array, m_Resource, 0, 0);
  cudaSurfaceObject_t textureSurface;
  cudaResourceDesc resourceDesc{};
  resourceDesc.res.array.array = array;
  resourceDesc.resType = cudaResourceTypeArray;
  cudaCreateSurfaceObject(&textureSurface, &resourceDesc);
  launchResliceKernel(textureSurface);
  cudaGraphicsUnmapResources(1, &m_Resource);
}

void ImageResliceFilterCuda::uploadVolume(std::shared_ptr<Volume> volume)
{
  if (!volume)
  {
    SPDLOG_DEBUG("volume is nullptr: {}", volume == nullptr);
    return;
  }
  if (!volume->m_Data)
  {
    SPDLOG_DEBUG("volume data is nullptr");
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
  SPDLOG_DEBUG(cudaGetErrorString(err));

  m_hVolume = volume;
}
void ImageResliceFilterCuda::uploadPlane(std::shared_ptr<Plane> plane)
{
  // upload plane
  cudaError_t err;
  if (!m_dPlane)
  {
    err = cudaMalloc(&m_dPlane, sizeof(Plane));
    SPDLOG_DEBUG(cudaGetErrorString(err));
  }

  err = cudaMemcpy(m_dPlane, plane.get(), sizeof(Plane), cudaMemcpyHostToDevice);
  SPDLOG_DEBUG(cudaGetErrorString(err));
}

void ImageResliceFilterCuda::destroyResources()
{
  if (m_dVolume || m_dPlane)
  {
    auto err = cudaFree(m_dVolume);
    SPDLOG_DEBUG(cudaGetErrorString(err));
    err = cudaFree(m_dPlane);
    SPDLOG_DEBUG(cudaGetErrorString(err));
    err = cudaDestroyTextureObject(m_Texture);
    SPDLOG_DEBUG(cudaGetErrorString(err));
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
void ImageResliceFilterCuda::launchResliceKernel(cudaSurfaceObject_t textureSurface)
{
  switch (m_hVolume->m_DataType)
  {
    case DataType::CHAR:
      return launchResliceKernelImpl<DataType::CHAR>(textureSurface);
    case DataType::UNSIGNED_CHAR:
      return launchResliceKernelImpl<DataType::UNSIGNED_CHAR>(textureSurface);
    case DataType::SHORT:
      return launchResliceKernelImpl<DataType::SHORT>(textureSurface);
    case DataType::UNSIGNED_SHORT:
      return launchResliceKernelImpl<DataType::UNSIGNED_SHORT>(textureSurface);
    case DataType::FLOAT:
      return launchResliceKernelImpl<DataType::FLOAT>(textureSurface);
    default:
      throw std::runtime_error("unsupported type");
  }
}

template <DataType dt>
void ImageResliceFilterCuda::launchResliceKernelImpl(cudaSurfaceObject_t textureSurface)
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
      <<<gridSize, blockSize>>>(m_dVolume, m_Texture, m_dPlane, textureSurface, m_WindowLevel);

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
  SPDLOG_DEBUG(cudaGetErrorString(err));
  if (err != cudaSuccess)
  {
    std::ostringstream oss;

    oss << "params: " << std::endl
        << "grid size: " << gridSize.x << " " << gridSize.y << " " << gridSize.z << std::endl
        << "type: " << typeid(type).name() << std::endl
        << "m_dVolume address: " << m_dVolume << std::endl
        << "m_Texture id: " << m_Texture << std::endl
        << "m_dPlane address: " << m_dPlane << std::endl
        << std::endl;
    SPDLOG_ERROR(oss.str());
  }
}
