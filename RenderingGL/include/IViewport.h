#ifndef INTERFACE_VIEWPORT_H
#define INTERFACE_VIEWPORT_H
#include "AbstractCamera.h"
#include <vtkSmartPointer.h>
class IViewport
{
public:
  virtual ~IViewport() = default;
  virtual void render() = 0;

  void setCamera(std::shared_ptr<AbstractCamera> camera) { mCamera = std::move(camera); }

protected:
  std::shared_ptr<AbstractCamera> mCamera;
};
#endif // INTERFACE_VIEWPORT_H
