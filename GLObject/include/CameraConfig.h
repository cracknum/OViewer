#ifndef CAMERA_CONFIG_H
#define CAMERA_CONFIG_H
#include <glm/detail/type_quat.hpp>
#include <glm/glm.hpp>

namespace CameraParams
{
enum class ProjectType : unsigned int
{
  PERSPECTIVE,
  ORTHOGONAL
};

struct ProjectConfig
{
  // project 相关参数
  float m_FovY;
  float m_Near;
  float m_Far;
  float m_Aspect;
  float m_Width;
  float m_Height;
  ProjectType m_ProjectType;

  ProjectConfig()
    : m_FovY(60.0f)
    , m_Near(0.1f)
    , m_Far(1000.0f)
    , m_Aspect(16.0f / 9.0f)
    , m_Width(1000.0f)
    , m_Height(1000.0f)
    , m_ProjectType(ProjectType::ORTHOGONAL)
  {
  }
};
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
}

#endif // CAMERA_CONFIG_H
