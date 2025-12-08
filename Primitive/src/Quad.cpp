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
  }
};

Quad::Quad(
  Functions* functions, std::shared_ptr<ShaderManager> shaderManager, const QuadConfig& config)
{
  m_Impl = std::make_unique<Impl>(functions, shaderManager, config);
}

Quad::~Quad() = default;

void Quad::setVertices(const std::shared_ptr<Vertices>& vertices)
{
  Superclass::setVertices(vertices);
  m_Impl->m_Data->createBuffer(*(vertices.get()));
}

void Quad::draw()
{
  m_Impl->m_Shader->use();
  bool setResult = false;
  setResult = m_Impl->m_Shader->setMat4(mModelMatrix, "modelMatrix");
  if (!setResult)
  {
    SPDLOG_ERROR("modelMatrix set error");
  }

  setResult = m_Impl->m_Shader->setMat4(mViewMatrix, "viewMatrix");
  if (!setResult)
  {
    SPDLOG_ERROR("viewMatrix set error");
  }

  setResult = m_Impl->m_Shader->setMat4(mProjectMatrix, "projectMatrix");
  if (!setResult)
  {
    SPDLOG_ERROR("projectMatrix set error");
  }
  m_Impl->m_Data->draw(GL_TRIANGLES);
  m_Impl->m_Shader->unuse();
}
