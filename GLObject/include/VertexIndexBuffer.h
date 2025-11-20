#ifndef VERTEX_INDEX_BUFFER_OPENGL_H
#define VERTEX_INDEX_BUFFER_OPENGL_H

#include <memory>

struct Vertices;
class QOpenGLFunctions_4_4_Core;
class VertexIndexBuffer
{
public:
  VertexIndexBuffer(QOpenGLFunctions_4_4_Core* function);
  ~VertexIndexBuffer();
  void createBuffer(const Vertices& vertices);
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