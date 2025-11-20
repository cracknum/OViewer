#ifndef VERTICES_OPENGL_H
#define VERTICES_OPENGL_H
#include <QOpenGLContext>
#include <array>
#include <utility>
struct Vertices
{
  GLfloat* m_Data;
  GLuint m_DataSize;
  GLfloat* m_Indices;
  GLuint m_IndicesSize;
  using Attribute = std::pair<GLboolean, std::array<GLuint, 2>>;
  Attribute m_Normal;
  Attribute m_Texture;

  Vertices()
    : m_Data(nullptr)
    , m_DataSize(0)
    , m_Indices(nullptr)
    , m_IndicesSize(0)
  {
    m_Normal.first = false;
    m_Texture.first = false;
  }
};
#endif // VERTICES_OPENGL_H