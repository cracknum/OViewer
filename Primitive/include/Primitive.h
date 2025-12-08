#ifndef GL_PRIMITIVE_H
#define GL_PRIMITIVE_H

#include "Vertices.h"
#include <glm/glm.hpp>
#include <memory>

struct Vertices;

class Primitive
{
public:
  Primitive()
    : mModelMatrix(1.0f)
    , mViewMatrix(1.0f)
    , mProjectMatrix(1.0f)
  {
  }
  virtual ~Primitive() = default;
  virtual void setVertices(const std::shared_ptr<Vertices>& vertices) { mVertices = vertices; }
  virtual void setModelMatrix(const glm::mat4& modelMatrix) { mModelMatrix = modelMatrix; }
  virtual void setViewMatrix(const glm::mat4& viewMatrix) { mViewMatrix = viewMatrix; }
  virtual void setProjectMatrix(const glm::mat4& projectMatrix) { mProjectMatrix = projectMatrix; }
  virtual inline void draw() = 0;

protected:
  std::shared_ptr<Vertices> mVertices;
  glm::mat4 mModelMatrix;
  glm::mat4 mViewMatrix;
  glm::mat4 mProjectMatrix;
};
#endif // GL_PRIMITIVE_H