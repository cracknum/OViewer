#ifndef OPENGLACTOR_H
#define OPENGLACTOR_H
#include <Actor.h>
#include <memory>
class OpenGLActor final : public Actor
{
public:
  OpenGLActor();
  ~OpenGLActor() override;
  void render() override;

private:
  struct Private;
  std::unique_ptr<Private> mPrivate;
};

#endif // OPENGLACTOR_H
