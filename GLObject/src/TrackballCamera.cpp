#include "TrackballCamera.h"
#include "CameraConfig.h"
#include <glm/gtc/quaternion.hpp>
#include <spdlog/spdlog.h>

TrackBallCamera::TrackBallCamera(
  CameraConfigPointer cameraConfig, ProjectConfigPointer projectConfig)
  : AbstractCamera(cameraConfig, projectConfig)
{
  init();
}

TrackBallCamera::~TrackBallCamera() {}

void TrackBallCamera::updateCameraPose(CameraConfigPointer cameraConfig)
{
  auto config = std::static_pointer_cast<CameraParams::TrackballCameraConfig>(cameraConfig);

  glm::vec3 spherePoint = projectToSphere(config->m_CurrentMousePos);
  if (config->m_PreviousMousePos != glm::vec3(0.0f, 0.0f, 0.0f))
  {
    float radian =
      glm::acos(glm::clamp(glm::dot(spherePoint, config->m_PreviousMousePos), -1.0f, 1.0f));
    glm::vec3 axis = glm::cross(config->m_PreviousMousePos, spherePoint);

    if (glm::length(axis) > 1e-6)
    {
      glm::quat delta = glm::angleAxis(radian, axis);
      m_CameraPose->m_Rotate = glm::normalize(delta * m_CameraPose->m_Rotate);
    }
  }
  config->m_PreviousMousePos = spherePoint;

  glm::vec3 defaultEye = glm::vec3(0.0f, 0.0f, -config->m_Distance);
  glm::vec3 rotatedEye = m_CameraPose->m_Rotate * defaultEye;

  m_CameraPose->m_Position = config->m_Target + rotatedEye;
  m_CameraPose->m_Front = glm::normalize(config->m_Target - m_CameraPose->m_Position);

  // 动态选择 up 参考轴，避免极点问题
  glm::vec3 worldUp =
    glm::abs(m_CameraPose->m_Front.y) > 0.999f ? glm::vec3(0, 0, 1) : glm::vec3(0, 1, 0);
  m_CameraPose->m_Right = glm::normalize(glm::cross(m_CameraPose->m_Front, worldUp));
  m_CameraPose->m_Up = glm::normalize(glm::cross(m_CameraPose->m_Right, m_CameraPose->m_Front));
}

glm::vec3 TrackBallCamera::projectToSphere(const glm::vec2& point)
{
  // length2其实是到原点(0, 0)的长度的平方，如果length2 < 1.0f说明坐标在用z=1切割出来的圆的内部，
  // 用1.0f - std::sqrt(length2)直接可以求z值，因为如果一个点在球上说明x^2 + y^2 + z^2 = 1
  float length2 = point.x * point.x + point.y * point.y;
  if (length2 < 1.0f)
  {
    float nz = std::sqrt(1.0f - length2);
    return glm::vec3(point, nz);
  }

  // 如果不在球上就将其映射到z=0的切割面上
  float length = std::sqrt(length2);
  glm::vec3 ret(point, 0.0f);
  ret.x /= length;
  ret.y /= length;

  return ret;
}
