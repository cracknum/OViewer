#include "Camera.h"
#include "CameraConfig.h"
#include <glm/gtc/quaternion.hpp>

struct Camera::Impl final
{
  CameraConfig m_Config;
};
Camera::Camera(const CameraConfig& cameraConfig)
{
  m_Impl = std::make_unique<Impl>();
  m_Impl->m_Config = cameraConfig;
}
void Camera::setPosition(const glm::vec3& position)
{
  m_Impl->m_Config.m_Position = position;
}
void Camera::setUp(const glm::vec3& up)
{
  m_Impl->m_Config.m_Up = up;
  m_Impl->m_Config.m_Right = glm::cross(m_Impl->m_Config.m_Front, m_Impl->m_Config.m_Up);
}
void Camera::setFront(const glm::vec3& front)
{
  m_Impl->m_Config.m_Front = front;
  m_Impl->m_Config.m_Right = glm::cross(m_Impl->m_Config.m_Front, m_Impl->m_Config.m_Up);
}
void Camera::rotate(const glm::vec3& axis, float angle)
{
  const float radian = glm::radians(angle);
  m_Impl->m_Config.m_Rotate = glm::angleAxis(radian, axis) * m_Impl->m_Config.m_Rotate;

  constexpr auto localFront = glm::vec3(0.0f, 0.0f, -1.0f);
  constexpr auto localUp = glm::vec3(0.0f, 1.0f, 0.0f);

  m_Impl->m_Config.m_Front = glm::normalize(m_Impl->m_Config.m_Rotate * localFront);
  m_Impl->m_Config.m_Up = glm::normalize(m_Impl->m_Config.m_Rotate * localUp);
  m_Impl->m_Config.m_Right =
    glm::normalize(glm::cross(m_Impl->m_Config.m_Front, m_Impl->m_Config.m_Up));
}
void Camera::moveTo(const glm::vec3& targetPoint)
{
  m_Impl->m_Config.m_Position = targetPoint;
}
void Camera::moveDiff(const glm::vec3& diff)
{
  m_Impl->m_Config.m_Position += diff;
}
glm::mat4 Camera::getWorldMatrix() const
{
  const glm::mat4 rotate = glm::mat4_cast(m_Impl->m_Config.m_Rotate);
  const auto& position = m_Impl->m_Config.m_Position;
  return glm::translate(glm::mat4(1.0f), position) * rotate;
}
glm::mat4 Camera::getViewMatrix() const
{
  const auto& position = m_Impl->m_Config.m_Position;
  const auto& center = center + m_Impl->m_Config.m_Front;
  const auto& up = m_Impl->m_Config.m_Up;
  return glm::lookAt(position, center, up);
}