#ifndef VERTEX_INDEX_BUFFER_OPENGL_H
#define VERTEX_INDEX_BUFFER_OPENGL_H

#include "GLObjectExport.h"
#include <glad/glad.h>
#include <memory>

struct Vertices;
class GLOBJECT_API VertexIndexBuffer
{
public:
  explicit VertexIndexBuffer();
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
