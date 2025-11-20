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

  explicit Impl(QOpenGLFunctions_4_4_Core* function)
    : m_VBO(0)
    , m_VAO(0)
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
  deleteBuffer();
}

void VertexIndexBuffer::bind() const
{
  if (!m_Impl->m_VAO)
  {
    throw std::runtime_error("vao is nullptr");
  }
  m_Impl->m_Function->glBindVertexArray(m_Impl->m_VAO);
}

void VertexIndexBuffer::draw() const
{
  if (m_Impl->m_Vertices.m_Indices == nullptr || m_Impl->m_Vertices.m_IndicesSize == 0)
  {
    throw std::runtime_error("vertices' indices not set");
  }

  bind();
  m_Impl->m_Function->glDrawElements(
    GL_TRIANGLES, m_Impl->m_Vertices.m_IndicesSize, GL_UNSIGNED_INT, nullptr);
  unbind();
}

void VertexIndexBuffer::unbind() const
{
  m_Impl->m_Function->glBindVertexArray(0);
}

void VertexIndexBuffer::createBuffer(const Vertices& vertices) const
{
  m_Impl->m_Vertices = vertices;
  const bool isDataInvalid = vertices.m_Data == nullptr;
  const bool isDataSizeInvalid = vertices.m_DataSize == 0;
  const bool isIndicesInvalid = vertices.m_Indices == nullptr;
  const bool isIndicesSizeInvalid = vertices.m_IndicesSize == 0;
  if (isDataInvalid || isDataSizeInvalid || isIndicesInvalid || isIndicesSizeInvalid)
  {
    std::ostringstream oss;
    oss << "vertices setting error: status{dataValid:" << !isDataInvalid
        << " dataSizeValid: " << !isDataSizeInvalid << " indicesValid: " << !isIndicesInvalid
        << " indicesSizeValid: " << !isIndicesSizeInvalid << "}";
    throw std::runtime_error(oss.str());
  }

  m_Impl->m_Function->glGenVertexArrays(1, &m_Impl->m_VAO);
  m_Impl->m_Function->glBindVertexArray(m_Impl->m_VAO);

  m_Impl->m_Function->glGenBuffers(1, &m_Impl->m_VBO);
  m_Impl->m_Function->glBindBuffer(GL_ARRAY_BUFFER, m_Impl->m_VBO);
  m_Impl->m_Function->glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * m_Impl->m_Vertices.m_DataSize,
    m_Impl->m_Vertices.m_Data, GL_STATIC_DRAW);

  m_Impl->m_Function->glGenBuffers(1, &m_Impl->m_EBO);
  m_Impl->m_Function->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Impl->m_EBO);
  m_Impl->m_Function->glBufferData(GL_ELEMENT_ARRAY_BUFFER,
    m_Impl->m_Vertices.m_IndicesSize * sizeof(GLuint), m_Impl->m_Vertices.m_Indices,
    GL_STATIC_DRAW);

  m_Impl->m_Function->glEnableVertexAttribArray(0);

  GLsizei offset = 0;
  GLuint index = 0;
  GLsizei stride = m_Impl->m_Vertices.m_DataAttribute.second * sizeof(GLfloat);
  if (m_Impl->m_Vertices.m_NormalAttribute.first)
  {
    stride += m_Impl->m_Vertices.m_NormalAttribute.second * sizeof(GLfloat);
  }
  if (m_Impl->m_Vertices.m_TextureAttribute.first)
  {
    stride += m_Impl->m_Vertices.m_TextureAttribute.second * sizeof(GLfloat); // NOLINT(*-narrowing-conversions)
  }
  m_Impl->m_Function->glVertexAttribPointer(
    index, m_Impl->m_Vertices.m_DataAttribute.second, GL_FLOAT, false, stride, nullptr);
  offset += m_Impl->m_Vertices.m_DataAttribute.second;
  index += 1;
  if (m_Impl->m_Vertices.m_NormalAttribute.first)
  {
    m_Impl->m_Function->glEnableVertexAttribArray(index);
    m_Impl->m_Function->glVertexAttribPointer(index, m_Impl->m_Vertices.m_NormalAttribute.second,
      GL_FLOAT, false, stride, reinterpret_cast<void*>(offset * sizeof(GLfloat)));
    offset += m_Impl->m_Vertices.m_NormalAttribute.second;
    index += 1;
  }
  if (m_Impl->m_Vertices.m_TextureAttribute.first)
  {
    m_Impl->m_Function->glEnableVertexAttribArray(index);
    m_Impl->m_Function->glVertexAttribPointer(index, m_Impl->m_Vertices.m_TextureAttribute.second,
      GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(offset * sizeof(GLfloat)));
    index += 1;
    offset += m_Impl->m_Vertices.m_TextureAttribute.second;
  }
}

void VertexIndexBuffer::deleteBuffer()
{
  if (!m_Impl->m_VAO)
  {
    return;
  }

  m_Impl->m_Function->glDisableVertexAttribArray(0);

  if (m_Impl->m_Vertices.m_NormalAttribute.first)
  {
    m_Impl->m_Function->glDisableVertexAttribArray(1);
  }
  if (m_Impl->m_Vertices.m_TextureAttribute.first)
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
