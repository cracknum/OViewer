#include "Line.h"
#include "LineConfig.h"
#include "PrimitiveType.h"
#include <ShaderProgramManager.h>
#include <ShaderProgram.h>
#include <VertexIndexBuffer.h>
#include <Vertices.h>
#include <iostream>
#include <spdlog/spdlog.h>
#include <utility>

struct Line::Impl final
{
  std::shared_ptr<ShaderProgramManager> m_ShaderManager;
  LineConfig m_LineConfig;
  ShaderProgramManager::ShaderProgramPointer m_ShaderProgram;
  std::unique_ptr<VertexIndexBuffer> m_VertexIndexBuffer;

  Impl(std::shared_ptr<ShaderProgramManager> shaderManager, const LineConfig& lineConfig)
    : m_ShaderManager(std::move(shaderManager))
    , m_LineConfig(lineConfig)
  {
    std::unordered_map<GLenum, std::string> shaderPathMap = { { GL_VERTEX_SHADER,
                                                     ":/shader/shader/Line.vert" },
      { GL_GEOMETRY_SHADER, ":/shader/shader/Line.geom" },
      { GL_FRAGMENT_SHADER, ":/shader/shader/Line.frag" } };
    m_ShaderProgram = m_ShaderManager->getShaderProgram(shaderPathMap);
   
    m_VertexIndexBuffer = std::make_unique<VertexIndexBuffer>();
    Vertices vertices;
    vertices.m_Data = new float[4]{ static_cast<float>(lineConfig.m_StartPoint.x),
      static_cast<float>(lineConfig.m_StartPoint.y), static_cast<float>(lineConfig.m_EndPoint.x),
      static_cast<float>(lineConfig.m_EndPoint.y) };

    vertices.m_DataSize = 4;
    vertices.m_Indices = new GLuint[2]{ 0, 1 };
    vertices.m_IndicesSize = 2;
    vertices.m_PointAttribute = Vertices::Attribute(GL_TRUE, 2);
    m_VertexIndexBuffer->createBuffer(vertices);
  }
};

Line::Line(const std::shared_ptr<ShaderProgramManager>& shaderManager, const LineConfig& lineConfig)
{
  m_Impl = std::make_unique<Impl>(shaderManager, lineConfig);
  setColor(m_Impl->m_LineConfig);
  setWidth(m_Impl->m_LineConfig);
}
Line::~Line() = default;

void Line::draw()
{
  m_Impl->m_ShaderProgram->use();
  bool setResult = false;
  setResult = m_Impl->m_ShaderProgram->setInt1(m_Impl->m_LineConfig.m_LineWidth, "lineWidth");
  if (!setResult)
  {
    SPDLOG_ERROR("lineWidth set error");
  }
  setResult = m_Impl->m_ShaderProgram->setVec4(m_Impl->m_LineConfig.m_Color, "lineColor");
  if (!setResult)
  {
    SPDLOG_ERROR("lineColor set error");
  }
  setResult = m_Impl->m_ShaderProgram->setVec2(m_Impl->m_LineConfig.m_ViewPortSize, "viewPortSize");
  if (!setResult)
  {
    SPDLOG_ERROR("viewPortSize set error");
  }

  m_Impl->m_VertexIndexBuffer->draw(GL_LINES);
  m_Impl->m_ShaderProgram->unuse();
}

void Line::setColor(const LineConfig& config)
{
  m_Impl->m_LineConfig.m_Color = config.m_Color;
}

void Line::setWidth(const LineConfig& config)
{
  m_Impl->m_LineConfig.m_LineWidth = config.m_LineWidth;
}

LineConfig& Line::getLineConfig()
{
  return m_Impl->m_LineConfig;
}
void Line::setLineConfig(const LineConfig& config)
{
  m_Impl->m_LineConfig = config;
}
void Line::setViewSize(const LineConfig::Size& size)
{
  m_Impl->m_LineConfig.m_ViewPortSize = size;
}
