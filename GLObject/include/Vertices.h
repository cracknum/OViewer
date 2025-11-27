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

  Vertices& operator=(Vertices&& vertices)
  { m_Data = vertices.m_Data;
    m_DataSize = vertices.m_DataSize;
    m_Indices = vertices.m_Indices;
    m_IndicesSize = vertices.m_IndicesSize;
    m_NormalAttribute = vertices.m_NormalAttribute;
    m_TextureAttribute = vertices.m_TextureAttribute;
    m_PointAttribute = vertices.m_PointAttribute;
    vertices.m_Data = nullptr;
    vertices.m_DataSize = 0;
    vertices.m_Indices = nullptr;
    vertices.m_IndicesSize = 0;

    return *this;
  }
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
      m_Data = nullptr;
    }
    if (m_Indices)
    {
      delete []m_Indices;
      m_IndicesSize = 0;
      m_Indices = nullptr;
    }
  }
};
#endif // VERTICES_OPENGL_H
