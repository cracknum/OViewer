#ifndef MAPPER_GL_H
#define MAPPER_GL_H
#include "RenderingGLExport.h"
#include <glm/glm.hpp>
#include <memory>

class RENDERINGGL_API Mapper
{
public:
  Mapper();
  virtual ~Mapper();

  virtual void render() = 0;
  void setViewMatrix(const glm::mat4& viewMatrix) { mViewMatrix = viewMatrix; }
  void setModelMatrix(const glm::mat4& modelMatrix) { mModelMatrix = modelMatrix; }
  void setProjectMatrix(const glm::mat4& projectMatrix) { mProjectMatrix = projectMatrix; }
  [[nodiscard]] glm::mat4 viewMatrix() const { return mViewMatrix; }
  [[nodiscard]] glm::mat4 modelMatrix() const { return mModelMatrix; }
  [[nodiscard]] glm::mat4 projectMatrix() const { return mProjectMatrix; }

protected:
  glm::mat4 mModelMatrix;
  glm::mat4 mViewMatrix;
  glm::mat4 mProjectMatrix;

private:
  struct Private;
  std::unique_ptr<Private> mPrivate;
};
#endif // MAPPER_GL_H