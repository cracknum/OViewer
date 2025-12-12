#include "OpenGLRenderer.h"
#include <Actor.h>
#include <vector>
struct OpenGLRenderer::Private
{
  std::vector<std::shared_ptr<Actor>> mActors;
};
void OpenGLRenderer::render()
{
  for (const auto& actor : mPrivate->mActors)
  {
    actor->render();
  }
}
OpenGLRenderer::OpenGLRenderer()
{
  mPrivate = std::make_unique<Private>();
}
OpenGLRenderer::~OpenGLRenderer() = default;