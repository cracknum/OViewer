#ifndef CAMERA_H
#define CAMERA_H
#include "AbstractCamera.h"
#include "GLObjectExport.h"
#include <glm/glm.hpp>
#include <memory>

class GLOBJECT_API FPSCamera : public AbstractCamera
{
public:
  explicit FPSCamera(CameraConfigPointer cameraConfig, ProjectConfigPointer projectConfig);
  ~FPSCamera();

protected:
  void updateCameraPose(CameraConfigPointer cameraConfig) override;
};

#endif // CAMERA_H
