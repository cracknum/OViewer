#ifndef CAMERA_CONFIG_H
#define CAMERA_CONFIG_H
#include <glm/detail/type_quat.hpp>
#include <glm/glm.hpp>

namespace CameraParams
{
enum class ProjectType : unsigned int
{
  NONE,
  PERSPECTIVE,
  ORTHOGONAL
};

struct ProjectConfig
{
  float m_Near;
  float m_Far;
  ProjectType m_ProjectType;

  ProjectConfig()
    : m_Near(0.1f)
    , m_Far(1000.0f)
  {
  }
  ~ProjectConfig() = default;
};

struct PerspectiveConfig : public ProjectConfig
{
  float m_FovY;
  float m_Aspect;

  PerspectiveConfig()
    : m_FovY(60.0f)
    , m_Aspect(16.0f / 9.0f)
  {
    m_ProjectType = ProjectType::PERSPECTIVE;
  }
};

struct OrthogonalConfig : public ProjectConfig
{
  float m_Width;
  float m_Height;

  OrthogonalConfig()
    : m_Width(1000.0f)
    , m_Height(1000.0f)
  {
    m_ProjectType = ProjectType::ORTHOGONAL;
  }
};

struct CameraConfig
{
  /**
   * @brief 相机的位置
   */
  glm::vec3 m_Position;
  /**
   * @brief 旋转四元数
   */
  glm::quat m_Rotate;

  CameraConfig()
    : m_Position(0.0f, 0.0f, 0.0f)
    , m_Rotate(0.0f, 0.0f, 0.0f, 1.0f)
  {
  }

  ~CameraConfig() = default;
};

struct FPSCameraConfig : public CameraConfig
{
  /**
   * @brief 旋转轴
   */
  glm::vec3 m_Axis;
  /**
   * @brief 旋转角
   */
  float m_Angle;

  FPSCameraConfig()
    : m_Axis(0.0f, 1.0f, 0.0f)
    , m_Angle(0.0f)
  {
  }
};

struct OrbitCameraConfig : public CameraConfig
{
  /**
   * @brief 绕y轴旋转角度
   */
  float m_Yaw;
  /**
   * @brief 绕x轴旋转角度
   */
  float m_Pitch;
  /**
   * @brief 绕target旋转
   */
  glm::vec3 m_Target;
  /**
   * @brief target到position的距离
   */
  float m_Distance;

  OrbitCameraConfig()
    : m_Yaw(0.0f)
    , m_Pitch(0.0f)
    , m_Target(0.0f)
    , m_Distance(0.0f)
  {
  }
};

struct CameraPose
{
  /**
   * @brief 相机的up方向
   */
  glm::vec3 m_Up;
  /**
   * @brief 相机的前向
   */
  glm::vec3 m_Front;
  /**
   * @brief 相机的右侧方向
   */
  glm::vec3 m_Right;
  /**
   * @brief 相机的位置
   */
  glm::vec3 m_Position;
  /**
   * @brief 旋转四元数
   */
  glm::quat m_Rotate;
};
}

#endif // CAMERA_CONFIG_H
