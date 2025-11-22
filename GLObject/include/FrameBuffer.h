#ifndef FRAMR_BUFFER_OPENGL_H
#define FRAMR_BUFFER_OPENGL_H
#include <memory>
#include "GLObjectExport.h"

class QOpenGLFunctions_4_4_Core;
class GLOBJECT_API FrameBuffer final
{
public:
  using Functions = QOpenGLFunctions_4_4_Core;
  FrameBuffer(Functions* functions, int width, int height);
  ~FrameBuffer();

  void bind() const;
  void unbind() const;

  void updateBufferSize(int width, int height);
  void deleteBuffer();
private:
  void createFrameBuffer();

private:
  struct Impl;
  std::unique_ptr<Impl> m_Impl;
};

#endif // FRAMR_BUFFER_OPENGL_H
