#include "SplitLine.h"
#include "LineConfig.h"
#include "ShaderProgram.h"
#include "VertexIndexBuffer.h"
#include "Vertices.h"
#include <iostream>
#include <spdlog/spdlog.h>

struct SplitLine::Impl final
{
  Functions* m_Functions;
  LineConfig m_LineConfig;
  std::unique_ptr<ShaderProgram> m_Shader;
  std::unique_ptr<VertexIndexBuffer> m_VertexIndexBuffer;

  Impl(Functions* functions, const LineConfig& lineConfig)
    : m_Functions(functions)
    , m_LineConfig(lineConfig)
  {
    ShaderProgram::ShaderPathMap shaderPathMap = { { GL_VERTEX_SHADER,
                                                     ":/shader/shader/Line.vert" },
      { GL_GEOMETRY_SHADER, ":/shader/shader/Line.geom" },
      { GL_FRAGMENT_SHADER, ":/shader/shader/Line.frag" } };
    m_Shader = std::make_unique<ShaderProgram>(functions, shaderPathMap);

    m_VertexIndexBuffer = std::make_unique<VertexIndexBuffer>(functions);
    Vertices vertices;
    vertices.m_Data = new float[4]{ static_cast<float>(lineConfig.m_StartPoint.x()),
      static_cast<float>(lineConfig.m_StartPoint.y()),
      static_cast<float>(lineConfig.m_EndPoint.x()),
      static_cast<float>(lineConfig.m_EndPoint.y()) };

    vertices.m_DataSize = 4;
    vertices.m_Indices = new GLuint[2]{ 0, 1 };
    vertices.m_IndicesSize = 2;
    m_VertexIndexBuffer->createBuffer(vertices);
  }
};

SplitLine::SplitLine(Functions* functions, const LineConfig& lineConfig)
{
  m_Impl = std::make_unique<Impl>(functions, lineConfig);
  setColor(m_Impl->m_LineConfig);
  setWidth(m_Impl->m_LineConfig);
}
SplitLine::~SplitLine() = default;

void SplitLine::mousePressEvent(QMouseEvent* event)
{
  (void)event;
}
void SplitLine::mouseReleaseEvent(QMouseEvent* event)
{
  (void)event;
}
void SplitLine::mouseDoubleClickEvent(QMouseEvent* event)
{
  (void)event;
}
void SplitLine::mouseMoveEvent(QMouseEvent* event)
{
  (void)event;
}
void SplitLine::wheelEvent(QWheelEvent* event)
{
  (void)event;
}
void SplitLine::keyPressEvent(QKeyEvent* event)
{
  (void)event;
}
void SplitLine::keyReleaseEvent(QKeyEvent* event)
{
  (void)event;
}
void SplitLine::draw()
{
  m_Impl->m_Shader->use();
  m_Impl->m_VertexIndexBuffer->draw();
  m_Impl->m_Shader->unuse();
}

void SplitLine::setColor(const LineConfig& config)
{
  if (!m_Impl->m_Shader)
  {
    spdlog::error("shader program is not exists");
    return;
  }
  m_Impl->m_Shader->use();
  bool setResult = m_Impl->m_Shader->setVec4(config.m_Color, "lineColor");
  if (!setResult)
  {
    spdlog::error("lineColor set error");
  }
  m_Impl->m_Shader->unuse();
}

void SplitLine::setWidth(const LineConfig& config)
{
  if (!m_Impl->m_Shader)
  {
    spdlog::error("shader program is not exists");
    return;
  }

  m_Impl->m_Shader->use();
  bool setResult = m_Impl->m_Shader->setFloat1(config.m_LineWidth, "lineWidth");
  if (!setResult)
  {
    spdlog::error("lineWidth set error");
  }
  m_Impl->m_Shader->unuse();
}
