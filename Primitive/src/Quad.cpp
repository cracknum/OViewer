#include "Quad.h"
#include "PrimitiveType.h"
#include <ShaderProgramManager.h>
#include <VertexIndexBuffer.h>
#include <ShaderProgram.h>
#include <Vertices.h>
#include <QOpenGLBuffer>
#include <spdlog/spdlog.h>

struct Quad::Impl
{
  std::shared_ptr<ShaderProgramManager> m_ShaderManager;
  std::unique_ptr<VertexIndexBuffer> m_Data;
  ShaderProgramManager::ShaderProgramPointer m_ShaderProgram;

  explicit Impl(const std::shared_ptr<ShaderProgramManager>& shaderManager)
    : m_ShaderManager(shaderManager)
  {
    // TODO: shader路径不正确
    std::unordered_map<GLenum, std::string> shaderPathMap = { { GL_VERTEX_SHADER,
                                                           ":/shader/shader/quad.vert" },
      { GL_FRAGMENT_SHADER, ":/shader/shader/quad.frag" } };
    if (!m_ShaderProgram)
    {
      m_ShaderProgram = shaderManager->getShaderProgram(shaderPathMap);
    }

    m_Data = std::make_unique<VertexIndexBuffer>();
  }
};

Quad::Quad(const std::shared_ptr<ShaderProgramManager>& shaderManager)
{
  m_Impl = std::make_unique<Impl>(shaderManager);
}

Quad::~Quad() = default;

void Quad::setVertices(const std::shared_ptr<Vertices>& vertices)
{
  Superclass::setVertices(vertices);
  m_Impl->m_Data->createBuffer(*(vertices));
}

void Quad::draw()
{
  m_Impl->m_ShaderProgram->use();
  bool setResult = false;
  setResult = m_Impl->m_ShaderProgram->setMat4(mModelMatrix, "modelMatrix");
  if (!setResult)
  {
    SPDLOG_ERROR("modelMatrix set error");
  }

  setResult = m_Impl->m_ShaderProgram->setMat4(mViewMatrix, "viewMatrix");
  if (!setResult)
  {
    SPDLOG_ERROR("viewMatrix set error");
  }

  setResult = m_Impl->m_ShaderProgram->setMat4(mProjectMatrix, "projectMatrix");
  if (!setResult)
  {
    SPDLOG_ERROR("projectMatrix set error");
  }
  m_Impl->m_Data->draw(GL_TRIANGLES);
  m_Impl->m_ShaderProgram->unuse();
}
