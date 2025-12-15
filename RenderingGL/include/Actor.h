#ifndef ACTOR_GL_H
#define ACTOR_GL_H
#include "RenderingGLExport.h"
#include "Mapper.h"
#include <memory>

class RENDERINGGL_API Actor
{
public:
  Actor();
  virtual ~Actor();
  void setMapper(const std::shared_ptr<Mapper>& mapper);
  virtual void render() = 0;
  std::shared_ptr<Mapper> mapper();

private:
  struct Private;
  std::unique_ptr<Private> mPrivate;
};
#endif // ACTOR_GL_H