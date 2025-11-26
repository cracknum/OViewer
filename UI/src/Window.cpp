#include "Window.h"
#include "Style.h"

#define NSIGHT_DEBUG

#if defined(NSIGHT_DEBUG)
#include <QTimer>
#endif

inline Window::Window(QWidget* parent)
  : QVTKOpenGLNativeWidget(parent)
{
}
Window::~Window() = default;
