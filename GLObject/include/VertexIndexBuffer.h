#ifndef VERTEX_INDEX_BUFFER_OPENGL_H
#define VERTEX_INDEX_BUFFER_OPENGL_H

#include <memory>
#include "GLObjectExport.h"

struct Vertices;
class QOpenGLFunctions_4_4_Core;
class GLOBJECT_API VertexIndexBuffer
{
public:
  explicit VertexIndexBuffer(QOpenGLFunctions_4_4_Core* function);
  ~VertexIndexBuffer();
  void createBuffer(const Vertices& vertices) const;
  void draw();

private:
  void bind();
  void unbind();
  void deleteBuffer();

private:
  struct Impl;
  std::unique_ptr<Impl> m_Impl;
};
#endif // VERTEX_INDEX_BUFFER_OPENGL_H
