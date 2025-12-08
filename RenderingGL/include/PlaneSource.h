#ifndef PLANE_SOURCE_H
#define PLANE_SOURCE_H
#include "Vertices.h"
#include <memory>
#include <glm/glm.hpp>
#include "RenderingGLExport.h"

class RENDERINGGL_API PlaneSource final
{
public:
  PlaneSource();
  ~PlaneSource();

  void setOrigin(double origin[3]);
  void setPoint1(double point[3]);
  void setPoint2(double point[3]);

  std::shared_ptr<Vertices> polyData();
  glm::mat4 indexToWorldMatrix();
  void updateGeometry();
private:

private:
  struct Private;
  std::unique_ptr<Private> mPrivate;
};
#endif // PLANE_SOURCE_H