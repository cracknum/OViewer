#include "ViewWindow.h"
#include <QOpenGLFunctions_4_4_Core>

ViewWindow::ViewWindow(
  QOpenGLFunctions_4_4_Core* function, const ViewWindowConfig& config, QObject* parent)
  : ViewWindowBase(function, config, parent)
{
}

void ViewWindow::drawWindow()
{
  const auto& viewPortConfig = m_Config.m_ViewPort;
  const auto& background = m_Config.m_BackgroundColor;
  const auto& viewPortSize = m_Config.m_ViewPortSize;

  m_Function->glEnable(GL_SCISSOR_TEST);
  m_Function->glScissor(viewPortSize[0], viewPortSize[1], viewPortSize[2], viewPortSize[3]);
  m_Function->glViewport(viewPortSize[0], viewPortSize[2], viewPortSize[2], viewPortSize[3]);
  m_Function->glClearColor(background.x(), background.y(), background.z(), background.w());
  m_Function->glClear(GL_COLOR_BUFFER_BIT);
  m_Function->glDisable(GL_SCISSOR_TEST);
}
