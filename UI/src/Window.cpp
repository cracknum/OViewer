#include "Window.h"
#include "Style.h"
#include "ViewWindow.h"
#include "ViewWindowConfig.h"

struct Window::Impl
{
  ViewWindowVector m_ViewWindows;
};

inline Window::Window(QWidget* parent)
  : QOpenGLWidget(parent)
{
  m_Impl = std::make_unique<Impl>();
}
inline Window::~Window() {}
void Window::addViewWindow(const ViewWindowConfig& config)
{
  auto* viewWindow = new ViewWindow(this->context(), config, this);
  m_Impl->m_ViewWindows.push_back(viewWindow);
}
void Window::initializeGL()
{
  this->initializeOpenGLFunctions();
  for (auto* viewWindow : m_Impl->m_ViewWindows)
  {
    if (!viewWindow->isContextVaild())
    {
      viewWindow->setContext(this->context());
    }
  }

  glClearColor(Style::WindowBackground[0], Style::WindowBackground[1], Style::WindowBackground[2],
    Style::WindowBackground[3]);
}
void Window::resizeGL(int w, int h)
{
  glViewport(0, 0, w, h);
}
void Window::paintGL()
{
  glClear(GL_COLOR_BUFFER_BIT);
}

void Window::mousePressEvent(QMouseEvent* event)
{
  QWidget::mousePressEvent(event);
}
void Window::mouseReleaseEvent(QMouseEvent* event)
{
  QWidget::mouseReleaseEvent(event);
}
void Window::mouseDoubleClickEvent(QMouseEvent* event)
{
  QWidget::mouseDoubleClickEvent(event);
}
void Window::mouseMoveEvent(QMouseEvent* event)
{
  QWidget::mouseMoveEvent(event);
}
void Window::wheelEvent(QWheelEvent* event)
{
  QWidget::wheelEvent(event);
}
void Window::keyPressEvent(QKeyEvent* event)
{
  QWidget::keyPressEvent(event);
}
void Window::keyReleaseEvent(QKeyEvent* event)
{
  QWidget::keyReleaseEvent(event);
}