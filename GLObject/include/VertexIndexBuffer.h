#ifndef VERTEX_INDEX_BUFFER_OPENGL_H
#define VERTEX_INDEX_BUFFER_OPENGL_H

#include <memory>
#include "GLObjectExport.h"
#include <qopenglfunctions_4_4_core.h>

struct Vertices;
class GLOBJECT_API VertexIndexBuffer
{
public:
  explicit VertexIndexBuffer(QOpenGLFunctions_4_4_Core* function);
  ~VertexIndexBuffer();
  void createBuffer(Vertices& vertices) const;
  void draw(GLenum drawMode);

private:
  void bind();
  void unbind();
  void deleteBuffer();

private:
  struct Impl;
  std::unique_ptr<Impl> m_Impl;
};
#endif // VERTEX_INDEX_BUFFER_OPENGL_H
