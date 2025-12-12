#ifndef RENDERER_RENDER_FRAMEWORK_H
#define RENDERER_RENDER_FRAMEWORK_H
#include <memory>
#include "RenderingGLExport.h"
class Actor;
class RENDERINGGL_API Renderer
{
public:
  Renderer();
  virtual ~Renderer();
  virtual void render() = 0;
  void addActor(const std::shared_ptr<Actor>& actor);
  void removeActor();

private:
  struct Private;
  std::unique_ptr<Private> mPrivate;
};
#endif