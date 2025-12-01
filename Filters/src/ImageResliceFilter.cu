#include "ImageResliceFilter.cuh"

#include <stdexcept>

namespace FilterKernel
{
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

  glm::vec3 worldPlanePixel = plane->m_Origin + plane->m_Right * static_cast<float>(x) +
    plane->m_Bottom * static_cast<float>(y);

  glm::vec3 volumeIndex = volume->m_WorldToIndex * glm::vec4(worldPlanePixel, 1.0f);

  auto data = tex3D<ComponentType>(texture, volumeIndex.x, volumeIndex.y, volumeIndex.z);
  pixels[y * plane->m_Width + x] = data;
}
}

ImageResliceFilter::ImageResliceFilter()
  : m_Texture(0)
  , m_dVolume(nullptr)
  , m_dPlane(nullptr)
{
}

ImageResliceFilter::~ImageResliceFilter()
{
  destroyResources();
}

void ImageResliceFilter::setVolume(std::shared_ptr<Volume> volume)
{
  destroyResources();

  m_hVolume = volume;
}
Volume* ImageResliceFilter::getVolume()
{
  return m_hVolume.get();
}

void ImageResliceFilter::setPlane(std::shared_ptr<Plane> plane)
{
  destroyResources();
  m_hPlane = plane;
}
Plane* ImageResliceFilter::getPlane()
{
  return m_hPlane.get();
}

void ImageResliceFilter::doFilter()
{
  if (m_dVolume == nullptr || m_hPlane == nullptr)
  {
    uploadVolume(m_hVolume, m_hPlane);
  }

  launchResliceKernel();
}

void ImageResliceFilter::uploadVolume(std::shared_ptr<Volume> volume, std::shared_ptr<Plane> plane)
{
  if (!volume || !plane)
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
  cudaMallocHost(&m_dVolume, sizeof(Volume));
  cudaMemcpy(m_dVolume, volume.get(), sizeof(Volume), cudaMemcpyHostToDevice);

  // upload volume data to texture
  cudaExtent extent = make_cudaExtent(volume->m_Dimensions.x * getDataTypeSize(volume->m_DataType),
    volume->m_Dimensions.y, volume->m_Dimensions.z);

  cudaArray_t dataArray{};
  cudaChannelFormatDesc channelFormat = createChannelFormat();
  cudaMalloc3DArray(&dataArray, &channelFormat, extent);
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
  textureDesc.filterMode = cudaTextureFilterMode::cudaFilterModeLinear;
  textureDesc.readMode = cudaTextureReadMode::cudaReadModeElementType;

  cudaCreateTextureObject(&m_Texture, &desc, &textureDesc, nullptr);

  // upload plane
  cudaMallocHost(&m_dPlane, sizeof(Plane));
  cudaMemcpy(m_dPlane, plane.get(), sizeof(Plane), cudaMemcpyHostToDevice);

  // create cache for plane pixels;
  cudaMallocHost(&m_Pixels, plane->m_Width * plane->m_Height * getDataTypeSize(volume->m_DataType));
  cudaMemset(m_Pixels, 0, plane->m_Width * plane->m_Height * getDataTypeSize(volume->m_DataType));

  m_hPlane = plane;
  m_hVolume = volume;
}

void ImageResliceFilter::destroyResources()
{
  if (m_dVolume || m_dPlane)
  {
    cudaFreeHost(m_dVolume);
    cudaFreeHost(m_dPlane);
    cudaDestroyTextureObject(m_Texture);
  }
}
cudaChannelFormatDesc ImageResliceFilter::createChannelFormat()
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
void ImageResliceFilter::launchResliceKernel()
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

template <DataType dt>
void ImageResliceFilter::launchResliceKernelImpl()
{
  using type = data_type<dt>;
  dim3 blockSize(128, 128, 1);
  dim3 gridSize((m_hPlane->m_Width + blockSize.x - 1) / blockSize.x,
    (m_hPlane->m_Height + blockSize.y - 1) / blockSize.y, 1);

  FilterKernel::resliceVolume<type>
    <<<gridSize, blockSize>>>(m_dVolume, m_Texture, m_dPlane, static_cast<type*>(m_Pixels));
  cudaDeviceSynchronize();
}
