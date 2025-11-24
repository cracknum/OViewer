#ifndef CAMERA_CONFIG_H
#define CAMERA_CONFIG_H
#include <glm/detail/type_quat.hpp>
#include <glm/glm.hpp>
struct CameraConfig
{
  glm::vec3 m_Position;
  glm::vec3 m_Up;
  glm::vec3 m_Front;
  glm::vec3 m_Right;
  glm::quat m_Rotate;

  CameraConfig()
    : m_Position(0.0f, 0.0f, 0.0f)
    , m_Up(0.0f, 1.0f, 0.0f)
    , m_Front(0.0f, 0.0f, -1.0f)
    , m_Right(glm::cross(m_Front, m_Up))
    , m_Rotate(0.0f, 0.0f, 0.0f, 1.0f)
  {
  }
};

#endif // CAMERA_CONFIG_H
