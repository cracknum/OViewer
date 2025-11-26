#include "FPSCamera.h"
#include "CameraConfig.h"
#include <glm/gtc/quaternion.hpp>
#include <spdlog/spdlog.h>

FPSCamera::FPSCamera(CameraConfigPointer cameraConfig, ProjectConfigPointer projectConfig)
  : AbstractCamera(cameraConfig, projectConfig)
{
  init();
}

FPSCamera::~FPSCamera() = default;

void FPSCamera::updateCameraPose(CameraConfigPointer cameraConfig)
{
  auto fpsConfig = std::static_pointer_cast<CameraParams::FPSCameraConfig>(cameraConfig);
  auto angle = fpsConfig->m_Angle;
  auto axis = fpsConfig->m_Axis;
  const float radian = glm::radians(angle);
  m_CameraPose->m_Rotate = glm::angleAxis(radian, axis) * m_CameraPose->m_Rotate;

  constexpr auto localFront = glm::vec3(0.0f, 0.0f, -1.0f);
  constexpr auto localUp = glm::vec3(0.0f, 1.0f, 0.0f);

  m_CameraPose->m_Front = glm::normalize(m_CameraPose->m_Rotate * localFront);
  m_CameraPose->m_Up = glm::normalize(m_CameraPose->m_Rotate * localUp);
  m_CameraPose->m_Right = glm::normalize(glm::cross(m_CameraPose->m_Front, m_CameraPose->m_Up));

  SPDLOG_INFO("front: {} {} {}", m_CameraPose->m_Rotate.x, m_CameraPose->m_Rotate.y, m_CameraPose->m_Rotate.z, m_CameraPose->m_Rotate.w);
}