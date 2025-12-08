#include "VertexIndexBuffer.h"
#include "Vertices.h"
#include <glm/glm.hpp>
#include <sstream>

struct VertexIndexBuffer::Impl
{
  GLuint m_VBO;
  GLuint m_VAO;
  GLuint m_EBO;
  Vertices m_Vertices;
  glm::mat4 m_IndexToWorldMatrix;

  explicit Impl()
    : m_VBO(0)
    , m_VAO(0)
    , m_EBO(0)
    , m_IndexToWorldMatrix(1.0f)
  {
  }
};

VertexIndexBuffer::VertexIndexBuffer()
{
  m_Impl = std::make_unique<Impl>();
}

VertexIndexBuffer::~VertexIndexBuffer()
{
  deleteBuffer();
}

void VertexIndexBuffer::bind()
{
  if (!m_Impl->m_VAO)
  {
    throw std::runtime_error("vao is nullptr");
  }
  glBindVertexArray(m_Impl->m_VAO);
}

void VertexIndexBuffer::draw(GLenum drawMode)
{
  if (m_Impl->m_Vertices.m_Indices == nullptr || m_Impl->m_Vertices.m_IndicesSize == 0)
  {
    throw std::runtime_error("vertices' indices not set");
  }

  bind();

  glDrawElements(drawMode, m_Impl->m_Vertices.m_IndicesSize, GL_UNSIGNED_INT, nullptr);

  unbind();
}

void VertexIndexBuffer::unbind()
{
  glBindVertexArray(0);
}

void VertexIndexBuffer::createBuffer(Vertices& vertices) const
{
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
  m_Impl->m_Vertices = std::move(vertices);

  glGenVertexArrays(1, &m_Impl->m_VAO);
  glBindVertexArray(m_Impl->m_VAO);

  glGenBuffers(1, &m_Impl->m_VBO);
  glBindBuffer(GL_ARRAY_BUFFER, m_Impl->m_VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * m_Impl->m_Vertices.m_DataSize,
    m_Impl->m_Vertices.m_Data, GL_STATIC_DRAW);

  glGenBuffers(1, &m_Impl->m_EBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Impl->m_EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Impl->m_Vertices.m_IndicesSize * sizeof(GLuint),
    m_Impl->m_Vertices.m_Indices, GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);

  GLsizei offset = 0;
  GLuint index = 0;
  GLsizei stride = m_Impl->m_Vertices.m_PointAttribute.second * sizeof(GLfloat);
  if (m_Impl->m_Vertices.m_NormalAttribute.first)
  {
    stride += m_Impl->m_Vertices.m_NormalAttribute.second * sizeof(GLfloat);
  }
  if (m_Impl->m_Vertices.m_TextureAttribute.first)
  {
    stride += m_Impl->m_Vertices.m_TextureAttribute.second * sizeof(GLfloat);
  }

  glVertexAttribPointer(
    index, m_Impl->m_Vertices.m_PointAttribute.second, GL_FLOAT, false, stride, nullptr);
  offset += m_Impl->m_Vertices.m_PointAttribute.second;
  index += 1;
  if (m_Impl->m_Vertices.m_NormalAttribute.first)
  {
    glEnableVertexAttribArray(index);
    glVertexAttribPointer(index, m_Impl->m_Vertices.m_NormalAttribute.second, GL_FLOAT, false,
      stride, reinterpret_cast<void*>(offset * sizeof(GLfloat)));
    offset += m_Impl->m_Vertices.m_NormalAttribute.second;
    index += 1;
  }
  if (m_Impl->m_Vertices.m_TextureAttribute.first)
  {
    glEnableVertexAttribArray(index);
    glVertexAttribPointer(index, m_Impl->m_Vertices.m_TextureAttribute.second, GL_FLOAT, GL_FALSE,
      stride, reinterpret_cast<void*>(offset * sizeof(GLfloat)));
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

  glDisableVertexAttribArray(0);

  if (m_Impl->m_Vertices.m_NormalAttribute.first)
  {
    glDisableVertexAttribArray(1);
  }
  if (m_Impl->m_Vertices.m_TextureAttribute.first)
  {
    glDisableVertexAttribArray(2);
  }

  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  glDeleteBuffers(1, &m_Impl->m_VBO);
  glDeleteBuffers(1, &m_Impl->m_EBO);
  glDeleteVertexArrays(1, &m_Impl->m_VAO);
}
