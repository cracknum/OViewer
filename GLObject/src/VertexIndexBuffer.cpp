#include "VertexIndexBuffer.h"
#include "Vertices.h"
#include <QOpenGLFunctions_4_4_Core>
#include <sstream>

struct VertexIndexBuffer::Impl
{
  GLuint m_VBO;
  GLuint m_VAO;
  GLuint m_EBO;
  Vertices m_Vertices;
  QOpenGLFunctions_4_4_Core* m_Function;

  Impl(QOpenGLFunctions_4_4_Core* function)
    : m_VAO(0)
    , m_VBO(0)
    , m_EBO(0)
    , m_Function(function)
  {
  }
};

VertexIndexBuffer::VertexIndexBuffer(QOpenGLFunctions_4_4_Core* context)
{
  m_Impl = std::make_unique<Impl>(context);
}

VertexIndexBuffer::~VertexIndexBuffer()
{
  if (!m_Impl->m_VAO)
  {
    return;
  }

  m_Impl->m_Function->glDisableVertexAttribArray(0);

  if (m_Impl->m_Vertices.m_Normal.first)
  {
    m_Impl->m_Function->glDisableVertexAttribArray(1);
  }
  if (m_Impl->m_Vertices.m_Texture.first)
  {
    m_Impl->m_Function->glDisableVertexAttribArray(2);
  }

  m_Impl->m_Function->glBindVertexArray(0);
  m_Impl->m_Function->glBindBuffer(GL_ARRAY_BUFFER, 0);
  m_Impl->m_Function->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  m_Impl->m_Function->glDeleteBuffers(1, &m_Impl->m_VBO);
  m_Impl->m_Function->glDeleteBuffers(1, &m_Impl->m_EBO);
  m_Impl->m_Function->glDeleteVertexArrays(1, &m_Impl->m_VAO);
}

void VertexIndexBuffer::bind()
{
  if (!m_Impl->m_VAO)
  {
    std::runtime_error("vao is nullptr");
  }
  m_Impl->m_Function->glBindVertexArray(m_Impl->m_VAO);
}

void VertexIndexBuffer::draw()
{
  if (m_Impl->m_Vertices.m_Indices == nullptr || m_Impl->m_Vertices.m_IndicesSize == 0)
  {
    std::runtime_error("vertices' indices not set");
  }

  bind();
  m_Impl->m_Function->glDrawElements(
    GL_TRIANGLES, m_Impl->m_Vertices.m_IndicesSize, GL_UNSIGNED_INT, nullptr);
  unbind();
}

void VertexIndexBuffer::unbind()
{
  m_Impl->m_Function->glBindVertexArray(0);
}

void VertexIndexBuffer::createBuffer(const Vertices& vertices)
{
  bool isDataInvalid = vertices.m_Data == nullptr;
  bool isDataSizeInvalid = vertices.m_DataSize == 0;
  bool isIndicesInvalid = vertices.m_Indices == nullptr;
  bool isIndicesSizeInvalid = vertices.m_IndicesSize == 0;
  if (isDataInvalid || isDataSizeInvalid || isIndicesInvalid || isIndicesSizeInvalid)
  {
    std::ostringstream oss;
    oss << "vertices setting error: status{dataVaild:" << !isDataInvalid
        << " dataSizeValid: " << !isDataSizeInvalid << " indicesValid: " << !isIndicesInvalid
        << " indicesSizeValid: " << !isIndicesSizeInvalid << "}";
    std::runtime_error(oss.str());
  }

  m_Impl->m_Function->glGenVertexArrays(1, &m_Impl->m_VAO);
  m_Impl->m_Function->glBindVertexArray(m_Impl->m_VAO);
  
  m_Impl->m_Function->glGenBuffers(1, &m_Impl->m_VBO);
  m_Impl->m_Function->glBindBuffer(GL_ARRAY_BUFFER, m_Impl->m_VBO);
  m_Impl->m_Function->glBufferData(GL_ARRAY_BUFFER, m_Impl->m_Vertices.m_DataSize, m_Impl->m_Vertices.m_Data, GL_STATIC_DRAW);
    
  // TODO: 继续完成buffer
}

void VertexIndexBuffer::deleteBuffer() {}
