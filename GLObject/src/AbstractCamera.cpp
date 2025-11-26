#include "AbstractCamera.h"
#include "CameraConfig.h"
#include <glm/gtc/quaternion.hpp>
#include <spdlog/spdlog.h>

AbstractCamera::AbstractCamera(CameraConfigPointer cameraConfig, ProjectConfigPointer projectConfig)
  : m_ProjectMatrix(1.0f)
  , m_ViewMatrix(1.0f)
  , m_WorldMatrix(1.0f)
{
  m_CameraConfig = cameraConfig;
  m_ProjectConfig = projectConfig;
  m_CameraPose = std::make_unique<CameraPose>();
}

void AbstractCamera::rotate(CameraConfigPointer cameraConfig)
{
  rotateInternal(cameraConfig);
  updateWorldMatrix();
  updateViewMatrix();
}
void AbstractCamera::moveTo(const glm::vec3& targetPoint)
{
  m_CameraPose->m_Position = targetPoint;
  updateWorldMatrix();
  updateViewMatrix();
}
void AbstractCamera::moveDiff(const glm::vec3& diff)
{
  m_CameraPose->m_Position += diff;
  updateWorldMatrix();
  updateViewMatrix();
}
glm::mat4 AbstractCamera::getWorldMatrix() const
{
  return m_WorldMatrix;
}
glm::mat4 AbstractCamera::getViewMatrix() const
{
  return m_ViewMatrix;
}

glm::mat4 AbstractCamera::getProjectMatrix() const
{
  return m_ProjectMatrix;
}

void AbstractCamera::updateViewMatrix()
{
  const auto& position = m_CameraPose->m_Position;
  const auto& center = position + m_CameraPose->m_Front;
  const auto& up = m_CameraPose->m_Up;
  m_ViewMatrix = glm::lookAt(position, center, up);
}

void AbstractCamera::updateWorldMatrix()
{
  const glm::mat4 rotate = glm::mat4_cast(m_CameraPose->m_Rotate);
  const auto& position = m_CameraPose->m_Position;
  m_WorldMatrix = translate(glm::mat4(1.0f), position) * rotate;
}

void AbstractCamera::updateProjectMatrix()
{
  if (m_ProjectConfig->m_ProjectType == CameraParams::ProjectType::PERSPECTIVE)
  {
    auto projectConfig = std::static_pointer_cast<CameraParams::PerspectiveConfig>(m_ProjectConfig);
    m_ProjectMatrix = glm::perspective(
      projectConfig->m_FovY, projectConfig->m_Aspect, projectConfig->m_Near, projectConfig->m_Far);
  }
  else
  {
    auto projectConfig = std::static_pointer_cast<CameraParams::OrthogonalConfig>(m_ProjectConfig);
    m_ProjectMatrix = glm::ortho(-projectConfig->m_Width * 0.5f, projectConfig->m_Width * 0.5f,
      -projectConfig->m_Height * 0.5f, projectConfig->m_Height * 0.5f, projectConfig->m_Near,
      projectConfig->m_Far);
  }
}

void AbstractCamera::init()
{
  updateCameraPose(m_CameraConfig);
  updateWorldMatrix();
  updateViewMatrix();
  updateProjectMatrix();
}
