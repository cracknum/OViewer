#include "OrbitCamera.h"
#include "CameraConfig.h"
#include <glm/gtc/quaternion.hpp>
#include <spdlog/spdlog.h>

OrbitCamera::OrbitCamera(CameraConfigPointer cameraConfig, ProjectConfigPointer projectConfig)
  : AbstractCamera(cameraConfig, projectConfig)
{
  init();
}

OrbitCamera::~OrbitCamera() {}

void OrbitCamera::updateCameraPose(CameraConfigPointer cameraConfig)
{
  auto orbitCameraConfig = std::static_pointer_cast<CameraParams::OrbitCameraConfig>(cameraConfig);
  auto yaw = glm::radians(orbitCameraConfig->m_Yaw);
  auto pitch = glm::radians(glm::clamp(orbitCameraConfig->m_Pitch, -89.0f, 89.0f));
  glm::vec3 front;
  front.x = cos(yaw) * cos(pitch);
  front.y = sin(pitch);
  front.z = sin(yaw) * cos(pitch);
  front = glm::normalize(front);

  m_CameraPose->m_Position = orbitCameraConfig->m_Target - orbitCameraConfig->m_Distance * front;
  m_CameraPose->m_Front = front;
  m_CameraPose->m_Right = glm::normalize(glm::cross(front, glm::vec3(0.0f, 1.0f, 0.0f)));
  m_CameraPose->m_Up = glm::normalize( glm::cross(m_CameraPose->m_Right, front));
}