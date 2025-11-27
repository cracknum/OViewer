#ifndef ABSTRACT_CAMERA_OPENGL_H
#define ABSTRACT_CAMERA_OPENGL_H
#include "GLObjectExport.h"
#include <glm/glm.hpp>
#include <memory>

namespace CameraParams
{
struct CameraConfig;
struct ProjectConfig;
struct CameraConfig;
struct CameraPose;
}
class GLOBJECT_API AbstractCamera
{
public:
  using CameraConfig = CameraParams::CameraConfig;
  using CameraPose = CameraParams::CameraPose;
  using ProjectConfig = CameraParams::ProjectConfig;
  using CameraConfigPointer = std::shared_ptr<CameraConfig>;
  using ProjectConfigPointer = std::shared_ptr<ProjectConfig>;

  explicit AbstractCamera(CameraConfigPointer cameraConfig, ProjectConfigPointer projectConfig);
  virtual ~AbstractCamera() = default;

  void rotate(CameraConfigPointer cameraConfig);
  void moveTo(const glm::vec3& targetPoint);
  void moveDiff(const glm::vec3& diff);

  [[nodiscard]] glm::mat4 getWorldMatrix() const;
  [[nodiscard]] glm::mat4 getViewMatrix() const;
  [[nodiscard]] glm::mat4 getProjectMatrix() const;

protected:
  /**
   * @brief 更新相机位姿
   */
  virtual void updateCameraPose(CameraConfigPointer cameraConfig) = 0;
  void updateViewMatrix();
  void updateWorldMatrix();
  void updateProjectMatrix();
  void init();

protected:
  ProjectConfigPointer m_ProjectConfig;
  CameraConfigPointer m_CameraConfig;
  glm::mat4 m_ProjectMatrix;
  glm::mat4 m_ViewMatrix;
  glm::mat4 m_WorldMatrix;
  std::unique_ptr<CameraParams::CameraPose> m_CameraPose;
};
#endif // ABSTRACT_CAMERA_OPENGL_H