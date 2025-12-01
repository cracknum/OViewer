#ifndef IMAGE_RESLICE_FILTER_CUDA_H
#define IMAGE_RESLICE_FILTER_CUDA_H
#include "Plane.h"
#include "Volume.h"
#include <cuda_runtime.h>
#include <device_launch_parameters.h>
#include <memory>
#include <spdlog/spdlog.h>

template <typename ComponentType>
struct ImageResliceFilter
{
public:
  using VolumeType = Volume<ComponentType>;
  __host__ ImageResliceFilter::ImageResliceFilter()
    : m_Texture(0)
    , m_dValume(nullptr)
    , m_dPlane(nullptr)
  {
  }

  __host__ ImageResliceFilter::~ImageResliceFilter() { destroyResources(); }

  __host__ void setVolume(std::shared_ptr<VolumeType> volume)
  {
    destroyResources();

    m_hVolume = volume;
  }
  __host__ VolumeType* getVolume() { return m_hVolume; }

  __host__ void setPlane(std::shared_ptr<Plane> plane)
  {
    destroyResources();
    m_hPlane = plane;
  }
  __host__ Plane* getPlane() { return m_hPlane; }

  __host__ void doFilter()
  {
    if (m_dValume == nullptr || m_hPlane == nullptr)
    {
      uploadVolume(m_hVolume, m_hPlane);
    }

    dim3 blockSize(128, 128, 1);
    dim3 gridSize((m_hPlane->m_Width + blockSize.x - 1) / blockSize.x,
      (m_hPlane->m_Height + blockSize.y - 1) / blockSize.y, 1);

    resliceVolume<<<gridSize, blockSize>>>(m_dValume, m_Texture, m_dPlane, m_Pixels);
    cudaDeviceSynchronize();
  }

private:
  friend class ImageResliceTest;
  __host__ void uploadVolume(std::shared_ptr<VolumeType> volume, std::shared_ptr<Plane> plane)
  {
    if (!volume || !plane)
    {
      SPDLOG_DEBUG(
        "volume is nullptr: {}, plane is nullptr: {}", volume == nullptr, plane == nullptr);
      return;
    }
    if (!volume->m_Data)
    {
      SPDLOG_DEBUG("volume data is nullptr");
      return;
    }

    // upload volume
    cudaMallocHost(&m_dValume, sizeof(VolumeType));
    cudaMemcpy(m_dValume, volume, sizeof(VolumeType), cudaMemcpyHostToDevice);

    // upload volume data to texture
    cudaExtent extent = make_cudaExtent(volume->m_Dimensions.x * sizeof(ComponentType),
      volume->m_Dimensions.y, volume->m_Dimensions.z);

    cudaArray_t dataArray{};
    cudaChannelFormatDesc channelFormat = cudaChannelFormatDesc<ComponentType>();
    cudaMalloc3DArray(&dataArray, channelFormat, extent);
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
    cudaMemcpy(m_dPlane, plane, sizeof(Plane), cudaMemcpyHostToDevice);

    // create cache for plane pixels;
    cudaMallocHost(&m_Pixels, plane->m_Width * plane->m_Height * sizeof(ComponentType));
    cudaMemset(m_Pixels, 0, plane->m_Width * plane->m_Height * sizeof(ComponentType));

    m_hPlane = plane;
    m_hVolume = volume;
  }
  __global__ void resliceVolume(
    Volume<ComponentType> volume, cudaTextureObject_t texture, Plane plane, ComponentType* pixels)
  {
    // already know plane bounds world coordinate
    uint x = blockDim.x * blockIdx.x + threadIdx.x;
    uint y = blockDim.y * blockIdx.y + threadIdx.y;

    if (x >= plane.m_Width || y >= plane.m_Height)
    {
      return;
    }

    glm::vec3 worldPlanePixel = plane.m_Origin + plane.m_Right * static_cast<float>(x) +
      plane.m_Bottom * static_cast<float>(y);

    glm::vec3 volumeIndex = volume.m_WorldToIndex * glm::vec4(worldPlanePixel, 1.0f);

    ComponentType data = tex3D<ComponentType>(texture, volumeIndex.x, volumeIndex.y, volumeIndex.z);
    pixels[y * plane.m_Width + x] = data;
  }

  void destroyResources()
  {
    if (m_dValume || m_dPlane)
    {
      cudaFreeHost(m_dValume);
      cudaFreeHost(m_dPlane);
      cudaDestroyTextureObject(m_Texture);
    }
  }

private:
  Volume<ComponentType>* m_dValume;
  Plane* m_dPlane;
  cudaTextureObject_t m_Texture;
  ComponentType* m_Pixels;
  std::shared_ptr<Volume<ComponentType>> m_hVolume;
  std::shared_ptr<Plane> m_hPlane;
};
#endif // IMAGE_RESLICE_FILTER_CUDA_H