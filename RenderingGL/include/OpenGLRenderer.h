#ifndef OPENGLRENDERER_H
#define OPENGLRENDERER_H
#include <Renderer.h>
#include <memory>

class OpenGLRenderer final : public Renderer
{
public:
  void render() override;
  OpenGLRenderer();
  ~OpenGLRenderer() override;

private:
  struct Private;
  std::unique_ptr<Private> mPrivate;
};

#endif // OPENGLRENDERER_H
