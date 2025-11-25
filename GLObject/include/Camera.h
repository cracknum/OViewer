#ifndef CAMERA_H
#define CAMERA_H
#include "GLObjectExport.h"
#include <glm/glm.hpp>
#include <memory>

namespace CameraParams
{
struct CameraConfig;
struct ProjectConfig;
}

class GLOBJECT_API Camera
{
public:
  explicit Camera(const CameraParams::CameraConfig& cameraConfig,
    const CameraParams::ProjectConfig& projectConfig);
  ~Camera();
  void setPosition(const glm::vec3& position);
  void setUp(const glm::vec3& up);
  void setFront(const glm::vec3& front);
  void rotate(const glm::vec3& axis, float angle);
  void moveTo(const glm::vec3& targetPoint);
  void moveDiff(const glm::vec3& diff);

  [[nodiscard]] glm::mat4 getWorldMatrix() const;
  [[nodiscard]] glm::mat4 getViewMatrix() const;
  [[nodiscard]] glm::mat4 getProjectMatrix() const;

private:
  void updateViewMatrix();
  void updateWorldMatrix();
  void updateProjectMatrix();

private:
  struct Impl;
  std::unique_ptr<Impl> m_Impl;
};

#endif // CAMERA_H
