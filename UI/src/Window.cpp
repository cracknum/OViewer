#include "Window.h"
#include "LineConfig.h"
#include "SplitLine.h"
#include "Style.h"
#include "ViewWindow.h"
#include "ViewWindowConfig.h"

struct Window::Impl
{
  ViewWindowVector m_ViewWindows;
  std::unique_ptr<SplitLine> m_HorizontalLine;
  std::unique_ptr<SplitLine> m_VerticalLine;
};

inline Window::Window(QWidget* parent)
  : QOpenGLWidget(parent)
{
  m_Impl = std::make_unique<Impl>();

  QSurfaceFormat format(this->format());
  format.setSwapBehavior(QSurfaceFormat::DoubleBuffer);
  this->setFormat(format);
}
inline Window::~Window() {}
void Window::addViewWindow(const ViewWindowConfig& config)
{
  auto* viewWindow = new ViewWindow(this, config, this);
  viewWindow->updateViewSize(ViewWindowConfig::WindowSize{ width(), height() });

  m_Impl->m_ViewWindows.push_back(viewWindow);
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

  LineConfig horizontalLineConfig, verticalLineConfig;
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

  m_Impl->m_HorizontalLine = std::make_unique<SplitLine>(this, horizontalLineConfig);
  m_Impl->m_VerticalLine = std::make_unique<SplitLine>(this, verticalLineConfig);
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
  m_Impl->m_HorizontalLine->draw();
  m_Impl->m_VerticalLine->draw();
}

void Window::mousePressEvent(QMouseEvent* event) {}
void Window::mouseReleaseEvent(QMouseEvent* event) {}
void Window::mouseDoubleClickEvent(QMouseEvent* event) {}
void Window::mouseMoveEvent(QMouseEvent* event) {}
void Window::wheelEvent(QWheelEvent* event) {}
void Window::keyPressEvent(QKeyEvent* event) {}
void Window::keyReleaseEvent(QKeyEvent* event) {}
