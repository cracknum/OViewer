#include "Quad.h"
#include "PrimitiveType.h"
#include "QuadConfig.h"
#include "ShaderManager.h"
#include "VertexIndexBuffer.h"
#include "Vertices.h"
#include <spdlog/spdlog.h>

struct Quad::Impl
{
  QuadConfig m_Config;
  std::shared_ptr<ShaderManager> m_ShaderManager;
  std::unique_ptr<VertexIndexBuffer> m_Data;
  ShaderManager::ShaderPointer m_Shader;
  Functions* m_Functions;
  glm::mat4 m_ModelMatrix;

  Impl(Functions* functions, std::shared_ptr<ShaderManager> shaderManager, const QuadConfig& config)
    : m_ShaderManager(shaderManager)
    , m_Config(config)
    , m_Functions(functions)
  {
    m_Config.m_Normal = glm::normalize(glm::cross(m_Config.m_U, m_Config.m_V));
    m_Config.m_D = glm::dot(m_Config.m_Origin, m_Config.m_Normal);
    m_Shader = shaderManager->getShader(PrimitiveType::QUAD);
    ShaderProgram::ShaderPathMap shaderPathMap = { { GL_VERTEX_SHADER,
                                                     ":/shader/shader/quad.vert" },
      { GL_FRAGMENT_SHADER, ":/shader/shader/quad.frag" } };
    if (!m_Shader)
    {
      m_Shader = shaderManager->registerShader(PrimitiveType::QUAD, shaderPathMap);
    }

    m_Data = std::make_unique<VertexIndexBuffer>(functions);

    Vertices vertices;
    // clang-format off
    vertices.m_Data = new GLfloat[12]
    {
      0, 0, 0,
      1, 0, 0,
      1, 1, 0,
      0, 1, 0
    };

    m_ModelMatrix = glm::mat4(
      glm::vec4(m_Config.m_U, 0.0f),
      glm::vec4(m_Config.m_V, 0.0f),
      glm::vec4(m_Config.m_Normal, 0.0f),
      glm::vec4(m_Config.m_Origin, 1.0f)
    );
    // clang-format on
    vertices.m_DataSize = 12;
    vertices.m_Indices = new GLuint[6]{ 0, 1, 3, 1, 2, 3 };
    vertices.m_IndicesSize = 6;
    vertices.m_PointAttribute = Vertices::Attribute(GL_TRUE, 3);
    m_Data->createBuffer(vertices);
  }
};

Quad::Quad(
  Functions* functions, std::shared_ptr<ShaderManager> shaderManager, const QuadConfig& config)
{
  m_Impl = std::make_unique<Impl>(functions, shaderManager, config);
}

Quad::~Quad() = default;

void Quad::mousePressEvent(QMouseEvent* event) {}

void Quad::mouseReleaseEvent(QMouseEvent* event) {}

void Quad::mouseDoubleClickEvent(QMouseEvent* event) {}

void Quad::mouseMoveEvent(QMouseEvent* event) {}

void Quad::wheelEvent(QWheelEvent* event) {}

void Quad::keyPressEvent(QKeyEvent* event) {}

void Quad::keyReleaseEvent(QKeyEvent* event) {}

void Quad::draw(const glm::mat4& viewMatrix, const glm::mat4& projectMatrix)
{
  m_Impl->m_Shader->use();
  bool setResult = false;
  setResult = m_Impl->m_Shader->setMat4(m_Impl->m_ModelMatrix, "modelMatrix");
  if (!setResult)
  {
    SPDLOG_ERROR("modelMatrix set error");
  }

  setResult = m_Impl->m_Shader->setMat4(viewMatrix, "viewMatrix");
  if (!setResult)
  {
    SPDLOG_ERROR("viewMatrix set error");
  }

  setResult = m_Impl->m_Shader->setMat4(projectMatrix, "projectMatrix");
  if (!setResult)
  {
    SPDLOG_ERROR("projectMatrix set error");
  }
  m_Impl->m_Data->draw(GL_TRIANGLES);
  m_Impl->m_Shader->unuse();
}
