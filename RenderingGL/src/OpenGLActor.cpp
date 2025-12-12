#include "OpenGLActor.h"

struct OpenGLActor::Private
{
};
inline OpenGLActor::OpenGLActor()
{
  mPrivate = std::make_unique<Private>();
}
inline OpenGLActor::~OpenGLActor() {}
inline void OpenGLActor::render()
{
  mapper()->render();
}