#ifndef VERTICES_OPENGL_H
#define VERTICES_OPENGL_H
#include <QOpenGLContext>
#include <array>
#include <utility>
#include "GLObjectExport.h"
struct GLOBJECT_API Vertices
{
  GLfloat* m_Data;
  GLsizeiptr m_DataSize;
  GLuint* m_Indices;
  GLsizei m_IndicesSize;
  // std::pair<enable, size> size: number of elements
  using Attribute = std::pair<GLboolean, GLsizei>;
  Attribute m_NormalAttribute;
  Attribute m_TextureAttribute;
  Attribute m_PointAttribute;

  Vertices()
    : m_Data(nullptr)
    , m_DataSize(0)
    , m_Indices(nullptr)
    , m_IndicesSize(0)
  {
    m_NormalAttribute.first = false;
    m_TextureAttribute.first = false;
    m_PointAttribute.first = true;
  }
  
  ~Vertices()
  {
    if (m_Data)
    {
      delete []m_Data;
      m_DataSize = 0;
    }
    if (m_Indices)
    {
      delete []m_Indices;
      m_IndicesSize = 0;
    }
  }
};
#endif // VERTICES_OPENGL_H
