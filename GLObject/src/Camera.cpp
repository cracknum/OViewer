#include "Camera.h"
#include "CameraConfig.h"
#include <glm/gtc/quaternion.hpp>

struct Camera::Impl final
{
  CameraParams::CameraConfig m_CameraConfig;
  CameraParams::ProjectConfig m_ProjectConfig;
  glm::mat4 m_ProjectMatrix;
  glm::mat4 m_ViewMatrix;
  glm::mat4 m_WorldMatrix;
  Impl()
    : m_ProjectMatrix(1.0f)
    , m_ViewMatrix(1.0f)
    , m_WorldMatrix(1.0f)
  {
  }
};
Camera::Camera(
  const CameraParams::CameraConfig& cameraConfig, const CameraParams::ProjectConfig& projectConfig)
{
  m_Impl = std::make_unique<Impl>();
  m_Impl->m_CameraConfig = cameraConfig;
  m_Impl->m_ProjectConfig = projectConfig;
  updateWorldMatrix();
  updateViewMatrix();
  updateProjectMatrix();
}
void Camera::setPosition(const glm::vec3& position)
{
  m_Impl->m_CameraConfig.m_Position = position;
  updateWorldMatrix();
  updateViewMatrix();
}
void Camera::setUp(const glm::vec3& up)
{
  m_Impl->m_CameraConfig.m_Up = up;
  m_Impl->m_CameraConfig.m_Right =
    glm::cross(m_Impl->m_CameraConfig.m_Front, m_Impl->m_CameraConfig.m_Up);
  updateWorldMatrix();
  updateViewMatrix();
}
void Camera::setFront(const glm::vec3& front)
{
  m_Impl->m_CameraConfig.m_Front = front;
  m_Impl->m_CameraConfig.m_Right =
    glm::cross(m_Impl->m_CameraConfig.m_Front, m_Impl->m_CameraConfig.m_Up);
  updateWorldMatrix();
  updateViewMatrix();
}
void Camera::rotate(const glm::vec3& axis, float angle)
{
  const float radian = glm::radians(angle);
  m_Impl->m_CameraConfig.m_Rotate = glm::angleAxis(radian, axis) * m_Impl->m_CameraConfig.m_Rotate;

  constexpr auto localFront = glm::vec3(0.0f, 0.0f, -1.0f);
  constexpr auto localUp = glm::vec3(0.0f, 1.0f, 0.0f);

  m_Impl->m_CameraConfig.m_Front = glm::normalize(m_Impl->m_CameraConfig.m_Rotate * localFront);
  m_Impl->m_CameraConfig.m_Up = glm::normalize(m_Impl->m_CameraConfig.m_Rotate * localUp);
  m_Impl->m_CameraConfig.m_Right =
    glm::normalize(glm::cross(m_Impl->m_CameraConfig.m_Front, m_Impl->m_CameraConfig.m_Up));
  updateWorldMatrix();
  updateViewMatrix();
}
void Camera::moveTo(const glm::vec3& targetPoint)
{
  m_Impl->m_CameraConfig.m_Position = targetPoint;
  updateWorldMatrix();
  updateViewMatrix();
}
void Camera::moveDiff(const glm::vec3& diff)
{
  m_Impl->m_CameraConfig.m_Position += diff;
  updateWorldMatrix();
  updateViewMatrix();
}
glm::mat4 Camera::getWorldMatrix() const
{
  return m_Impl->m_WorldMatrix;
}
glm::mat4 Camera::getViewMatrix() const
{
  return m_Impl->m_ViewMatrix;
}

glm::mat4 Camera::getProjectMatrix() const
{
  return m_Impl->m_ProjectMatrix;
}

void Camera::updateViewMatrix()
{
  const auto& position = m_Impl->m_CameraConfig.m_Position;
  const auto& center = position + m_Impl->m_CameraConfig.m_Front;
  const auto& up = m_Impl->m_CameraConfig.m_Up;
  m_Impl->m_ViewMatrix = glm::lookAt(position, center, up);
}

void Camera::updateWorldMatrix()
{
  const glm::mat4 rotate = glm::mat4_cast(m_Impl->m_CameraConfig.m_Rotate);
  const auto& position = m_Impl->m_CameraConfig.m_Position;
  m_Impl->m_WorldMatrix = glm::translate(glm::mat4(1.0f), position) * rotate;
}

void Camera::updateProjectMatrix()
{
  auto& projectConfig = m_Impl->m_ProjectConfig;
  if (projectConfig.m_ProjectType == CameraParams::ProjectType::PERSPECTIVE)
  {
	m_Impl->m_ProjectMatrix = glm::perspective(
    projectConfig.m_FovY, projectConfig.m_Aspect, projectConfig.m_Near, projectConfig.m_Far);
  }
  else
  {
	m_Impl->m_ProjectMatrix = glm::ortho(
		-projectConfig.m_Width * 0.5, projectConfig.m_Width * 0.5,
		-projectConfig.m_Height * 0.5, projectConfig.m_Height * 0.5
	);
  }
  
  
}
