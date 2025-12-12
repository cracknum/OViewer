#ifndef IMAGE_RESLICE_FILTER_CUDA_H
#define IMAGE_RESLICE_FILTER_CUDA_H
#include "Plane.h"
#include "Volume.h"
#include <cuda_runtime.h>
#include <device_launch_parameters.h>
#include <memory>
// #include <spdlog/spdlog.h>
#include "FiltersExport.h"

class FILTERS_API ImageResliceFilterCuda
{
public:
  ImageResliceFilterCuda::ImageResliceFilterCuda();

  ImageResliceFilterCuda::~ImageResliceFilterCuda();

  void setVolume(std::shared_ptr<Volume> volume);
  Volume* getVolume();

  void setPlane(std::shared_ptr<Plane> plane);
  Plane* getPlane();
  void setGLTexture(unsigned int glTexture);

  void setWindowLevel(float window, float level);

  void doFilter();

private:
  friend class ImageResliceTest;
  void uploadVolume(std::shared_ptr<Volume> volume);
  void uploadPlane(std::shared_ptr<Plane> plane);

  void destroyResources();
  cudaChannelFormatDesc createChannelFormat();
  void launchResliceKernel(cudaSurfaceObject_t textureSurface);
  template <DataType dt>
  void launchResliceKernelImpl(cudaSurfaceObject_t textureSurface);

private:
  Volume* m_dVolume;
  Plane* m_dPlane;
  cudaTextureObject_t m_Texture;
  std::shared_ptr<Volume> m_hVolume;
  std::shared_ptr<Plane> m_hPlane;
  unsigned int m_GLTexture;
  cudaGraphicsResource_t m_Resource;
  float2 m_WindowLevel;
};
#endif // IMAGE_RESLICE_FILTER_CUDA_H