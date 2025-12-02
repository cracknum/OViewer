#include "Window.h"
#include "Line.h"
#include "LineConfig.h"
#include "Quad.h"
#include "QuadConfig.h"
#include "ShaderManager.h"
#include "Style.h"
#include "ViewWindow.h"
#include "ViewWindowConfig.h"

#define NSIGHT_DEBUG

#if defined(NSIGHT_DEBUG)
#include <QTimer>
#endif

struct Window::Impl final
{
  ViewWindowVector m_ViewWindows;
  std::unique_ptr<Line> m_HorizontalLine;
  std::unique_ptr<Line> m_VerticalLine;
  std::shared_ptr<ShaderManager> m_ShaderManager;
#if defined(NSIGHT_DEBUG)
  QTimer* timer = nullptr;
#endif
};

inline Window::Window(QWidget* parent)
  : QOpenGLWidget(parent)
{
  m_Impl = std::make_unique<Impl>();

#if defined(NSIGHT_DEBUG)
  m_Impl->timer = new QTimer(this);
  connect(m_Impl->timer, &QTimer::timeout, this, [this]() { this->update(); });
  m_Impl->timer->start(100);
#endif

  QSurfaceFormat format(this->format());
  format.setSwapBehavior(QSurfaceFormat::DoubleBuffer);
  this->setFormat(format);
}

Window::~Window() = default;
void Window::addViewWindow(const ViewWindowConfig& config)
{
  auto* viewWindow = new ViewWindow(this, config, this);
  viewWindow->updateViewSize(ViewWindowConfig::WindowSize{ width(), height() });

  m_Impl->m_ViewWindows.push_back(viewWindow);
}

ViewWindowBase* Window::getViewWindow(ViewWindowConfig::IdType id) const
{
  auto it = std::find_if(m_Impl->m_ViewWindows.begin(), m_Impl->m_ViewWindows.end(),
    [id](ViewWindowBase* viewWindow) { return viewWindow->getWindowId() == id; });
  if (it != m_Impl->m_ViewWindows.end())
  {
    return *it;
  }

  return nullptr;
}

Window::ShaderManagerPointer Window::getShaderManager() const
{
  return m_Impl->m_ShaderManager;
}

void Window::initializeGL()
{
  this->initializeOpenGLFunctions();
  for (auto* viewWindow : m_Impl->m_ViewWindows)
  {
    if (!viewWindow->isContextVaild())
    {
      viewWindow->setFunction(this);
    }
  }

  LineConfig horizontalLineConfig{}, verticalLineConfig{};
  horizontalLineConfig.m_Color = LineConfig::Color(1.0, 1.0, 1.0, 1.0);
  horizontalLineConfig.m_StartPoint = LineConfig::Point(-1.0f, 0.0);
  horizontalLineConfig.m_EndPoint = LineConfig::Point(1.0f, 0.0);
  horizontalLineConfig.m_LineWidth = 5;
  horizontalLineConfig.m_ViewPortSize = LineConfig::Size(width(), height());

  verticalLineConfig.m_Color = LineConfig::Color(1.0, 1.0, 1.0, 1.0);
  verticalLineConfig.m_StartPoint = LineConfig::Point(0.0, -1.0);
  verticalLineConfig.m_EndPoint = LineConfig::Point(0.0, 1.0f);
  verticalLineConfig.m_LineWidth = 5;
  verticalLineConfig.m_ViewPortSize = LineConfig::Size(width(), height());

  m_Impl->m_ShaderManager = std::make_shared<ShaderManager>(this);

  m_Impl->m_HorizontalLine =
    std::make_unique<Line>(this, m_Impl->m_ShaderManager, horizontalLineConfig);
  m_Impl->m_VerticalLine =
    std::make_unique<Line>(this, m_Impl->m_ShaderManager, verticalLineConfig);

  auto* window = getViewWindow(ViewWindowConfig::IdType(0));

  QuadConfig quadConfig{};
  quadConfig.m_Origin = glm::vec3(-300.0f, -300.0f, -200.0f);
  quadConfig.m_U = glm::vec3(500.0f, 0.0f, 0.0f);
  quadConfig.m_V = glm::vec3(0.0f, 500.0f, 0.0f);

  std::shared_ptr<Quad> quad = std::make_shared<Quad>(this, m_Impl->m_ShaderManager, quadConfig);
  window->addPrimitive(quad);
}
void Window::resizeGL(int w, int h)
{
  glViewport(0, 0, w, h);
  for (auto* viewWindow : m_Impl->m_ViewWindows)
  {
    viewWindow->updateViewSize(ViewWindowConfig::WindowSize{ width(), height() });
  }

  if (m_Impl->m_HorizontalLine)
  {
    m_Impl->m_HorizontalLine->setViewSize(LineConfig::Size(width(), height()));
  }

  if (m_Impl->m_VerticalLine)
  {
    m_Impl->m_VerticalLine->setViewSize(LineConfig::Size(width(), height()));
  }
}
void Window::paintGL()
{
  glClearColor(Style::WindowBackground[0], Style::WindowBackground[1], Style::WindowBackground[2],
    Style::WindowBackground[3]);
  glClear(GL_COLOR_BUFFER_BIT);

  for (auto* viewWindow : m_Impl->m_ViewWindows)
  {
    viewWindow->update();
  }

  glViewport(0, 0, width(), height());
  m_Impl->m_HorizontalLine->draw(glm::mat4(1.0f), glm::mat4(1.0f));
  m_Impl->m_VerticalLine->draw(glm::mat4(1.0f), glm::mat4(1.0f));
}

void Window::mousePressEvent(QMouseEvent* event) {}
void Window::mouseReleaseEvent(QMouseEvent* event) {}
void Window::mouseDoubleClickEvent(QMouseEvent* event) {}
void Window::mouseMoveEvent(QMouseEvent* event) {}
void Window::wheelEvent(QWheelEvent* event) {}
void Window::keyPressEvent(QKeyEvent* event) {}
void Window::keyReleaseEvent(QKeyEvent* event) {}

void Window::openImage(DicomSeries* dicom)
{
	
}
