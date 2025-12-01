#ifndef IMAGE_RESLICE_FILTER_CUDA_H
#define IMAGE_RESLICE_FILTER_CUDA_H
#include "Plane.h"
#include "Volume.h"
#include <cuda_runtime.h>
#include <device_launch_parameters.h>
#include <memory>
// #include <spdlog/spdlog.h>
#include "FiltersExport.h"

class FILTERS_API ImageResliceFilter
{
public:
  ImageResliceFilter::ImageResliceFilter();

  ImageResliceFilter::~ImageResliceFilter();

  void setVolume(std::shared_ptr<Volume> volume);
  Volume* getVolume();

  void setPlane(std::shared_ptr<Plane> plane);
  Plane* getPlane();

  void doFilter();

private:
  friend class ImageResliceTest;
  void uploadVolume(std::shared_ptr<Volume> volume, std::shared_ptr<Plane> plane);

  void destroyResources();
  cudaChannelFormatDesc createChannelFormat();
  void launchResliceKernel();
  template <DataType dt>
  void launchResliceKernelImpl();

private:
  Volume* m_dVolume;
  Plane* m_dPlane;
  cudaTextureObject_t m_Texture;
  void* m_Pixels;
  std::shared_ptr<Volume> m_hVolume;
  std::shared_ptr<Plane> m_hPlane;
};
#endif // IMAGE_RESLICE_FILTER_CUDA_H