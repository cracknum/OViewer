#include "ViewWindow.h"
#include "InteractiveObject.h"
#include <QOpenGLFunctions_4_4_Core>
#include <spdlog/spdlog.h>


struct ViewWindow::Impl
{
  
};

ViewWindow::ViewWindow(
  QOpenGLFunctions_4_4_Core* function, const ViewWindowConfig& config, QObject* parent)
  : ViewWindowBase(function, config, parent)
{
  m_Impl = std::make_unique<Impl>();
}

void ViewWindow::drawWindow()
{
  const auto& viewPortConfig = m_Config.m_ViewPort;
  const auto& background = m_Config.m_BackgroundColor;
  const auto& viewPortSize = m_Config.m_ViewPortSize;

  m_Function->glViewport(viewPortSize[0], viewPortSize[1], viewPortSize[2], viewPortSize[3]);
  for (auto& primitive : m_Primitives)
  {
	primitive->draw();
  }
  
}

