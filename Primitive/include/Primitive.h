#ifndef GL_PRIMITIVE_H
#define GL_PRIMITIVE_H

#include "Vertices.h"
#include <glm/glm.hpp>
#include <memory>

class Vertices;

class Primitive
{
public:
  virtual ~Primitive() = default;
  virtual inline void setVertices(const std::shared_ptr<Vertices>& vertices)
  {
	mVertices = vertices;
  }
  virtual inline void setModelMatrix(const glm::mat4& modelMatrix)
  {
	mModelMatrix = modelMatrix;
  }
  virtual inline void setViewMatrix(const glm::mat4& viewMatrix)
  {
	mViewMatrix = viewMatrix;
  }
  virtual inline void setProjectMatrix(const glm::mat4& projectMatrix)
  {
	mProjectMatrix = projectMatrix;
  }
  virtual inline void draw() = 0;

protected:
  std::shared_ptr<Vertices> mVertices;
  glm::mat4 mModelMatrix;
  glm::mat4 mViewMatrix;
  glm::mat4 mProjectMatrix;
};
#endif // GL_PRIMITIVE_H