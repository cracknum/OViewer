#include "ViewWindow.h"
#include "Camera.h"
#include "CameraConfig.h"
#include "InteractiveObject.h"
#include <QOpenGLFunctions_4_4_Core>
#include <spdlog/spdlog.h>

#define DEBUG_CAMERA ;

struct ViewWindow::Impl
{
  std::unique_ptr<Camera> m_Camera;
  Impl()
  {
    m_Camera =
      std::make_unique<Camera>(CameraParams::CameraConfig(), CameraParams::ProjectConfig());
  }
};

ViewWindow::ViewWindow(
  QOpenGLFunctions_4_4_Core* function, const ViewWindowConfig& config, QObject* parent)
  : ViewWindowBase(function, config, parent)
{
  m_Impl = std::make_unique<Impl>();
}

void ViewWindow::drawWindow()
{
  const auto& viewPortSize = m_Config.m_ViewPortSize;

  m_Function->glViewport(viewPortSize[0], viewPortSize[1], viewPortSize[2], viewPortSize[3]);
  for (auto& primitive : m_Primitives)
  {
    primitive->draw(m_Impl->m_Camera->getViewMatrix(), m_Impl->m_Camera->getProjectMatrix());
  }
}
