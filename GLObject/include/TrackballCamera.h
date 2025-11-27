#ifndef TRACK_BALL_CAMERA_H
#define TRACK_BALL_CAMERA_H
#include "AbstractCamera.h"
#include <glm/glm.hpp>
#include "GLObjectExport.h"

class GLOBJECT_API TrackBallCamera : public AbstractCamera
{
public:
  explicit TrackBallCamera(CameraConfigPointer cameraConfig, ProjectConfigPointer projectConfig);
  ~TrackBallCamera();

protected:
  void updateCameraPose(CameraConfigPointer cameraConfig) override;

  /**
   * @brief 将[-1, 1]x[-1,1]的2D顶点映射到3D球上得到z坐标，从而得到球上的3D坐标
   * @details 
   * 	- 屏幕平面：相当于一个垂直于 Z 轴的平面，通常放置在 Z = 0 处；
   *	- 虚拟球体：半径为 1 的单位球体，中心位于原点 (0,0,0)；
   *	- 鼠标位置：在屏幕平面上的二维坐标 (x, y)，通过 projectToSphere 映射到球体表面的三维坐标 (sx, sy, sz)
   */
  glm::vec3 projectToSphere(const glm::vec2& point);
};
#endif // TRACK_BALL_CAMERA_H