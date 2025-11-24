#ifndef CAMERA_H
#define CAMERA_H
#include <glm/glm.hpp>
#include <memory>

struct CameraConfig;

class Camera
{
public:
  explicit Camera(const CameraConfig& cameraConfig);
  void setPosition(const glm::vec3& position);
  void setUp(const glm::vec3& up);
  void setFront(const glm::vec3& front);
  void rotate(const glm::vec3& axis, float angle);
  void moveTo(const glm::vec3& targetPoint);
  void moveDiff(const glm::vec3& diff);

  [[nodiscard]] glm::mat4 getWorldMatrix() const;
  [[nodiscard]] glm::mat4 getViewMatrix() const;

private:
  void updateRight();

private:
  struct Impl;
  std::unique_ptr<Impl> m_Impl;
};

#endif // CAMERA_H
