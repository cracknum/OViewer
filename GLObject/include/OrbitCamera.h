#ifndef ORBIT_CAMERA_OPENGL_H
#define ORBIT_CAMERA_OPENGL_H
#include "AbstractCamera.h"
#include "GLObjectExport.h"
class GLOBJECT_API OrbitCamera : public AbstractCamera
{
public:
  OrbitCamera(CameraConfigPointer cameraConfig, ProjectConfigPointer projectConfig);
  ~OrbitCamera();

protected:
  void updateCameraPose(CameraConfigPointer cameraConfig) override;
};

#endif