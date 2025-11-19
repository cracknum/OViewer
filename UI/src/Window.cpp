#include "Window.h"
#include "Style.h"
inline Window::Window(QWidget* parent): QWidget(parent)
{
  setStyleSheet(Style::WindowStyle);
}
inline Window::~Window() {}
inline void Window::mousePressEvent(QMouseEvent* event)
{
  QWidget::mousePressEvent(event);
}
inline void Window::mouseReleaseEvent(QMouseEvent* event)
{
  QWidget::mouseReleaseEvent(event);
}
inline void Window::mouseDoubleClickEvent(QMouseEvent* event)
{
  QWidget::mouseDoubleClickEvent(event);
}
inline void Window::mouseMoveEvent(QMouseEvent* event)
{
  QWidget::mouseMoveEvent(event);
}
inline void Window::wheelEvent(QWheelEvent* event)
{
  QWidget::wheelEvent(event);
}
inline void Window::keyPressEvent(QKeyEvent* event)
{
  QWidget::keyPressEvent(event);
}
inline void Window::keyReleaseEvent(QKeyEvent* event)
{
  QWidget::keyReleaseEvent(event);
}
inline void Window::enterEvent(QEvent* event)
{
  QWidget::enterEvent(event);
}
inline void Window::leaveEvent(QEvent* event)
{
  QWidget::leaveEvent(event);
}
inline void Window::resizeEvent(QResizeEvent* event)
{
  QWidget::resizeEvent(event);
}