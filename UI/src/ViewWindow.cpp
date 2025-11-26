#include "ViewWindow.h"
#include "CameraConfig.h"
#include "FPSCamera.h"
#include "InteractiveObject.h"
#include <QOpenGLFunctions_4_4_Core>
#include <spdlog/spdlog.h>

#define DEBUG_CAMERA ;
#if defined(DEBUG_CAMERA)
#include <QTimer>
#endif

struct ViewWindow::Impl
{
  std::unique_ptr<AbstractCamera> m_Camera;
#if defined(DEBUG_CAMERA)
  QTimer timer;
#endif
  Impl()
  {
    std::shared_ptr<CameraParams::CameraConfig> cameraConfig =
      std::make_shared<CameraParams::FPSCameraConfig>();
    std::shared_ptr<CameraParams::ProjectConfig> projectConfig =
      std::make_shared<CameraParams::OrthogonalConfig>();
    m_Camera = std::make_unique<FPSCamera>(cameraConfig, projectConfig);
  }
};

ViewWindow::ViewWindow(
  QOpenGLFunctions_4_4_Core* function, const ViewWindowConfig& config, QObject* parent)
  : ViewWindowBase(function, config, parent)
{
  m_Impl = std::make_unique<Impl>();
#if defined(DEBUG_CAMERA)
  connect(&m_Impl->timer, &QTimer::timeout, this, &ViewWindow::rotateCamera);
  m_Impl->timer.start(100);
#endif
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

void ViewWindow::rotateCamera()
{
  std::shared_ptr<CameraParams::FPSCameraConfig> config =
    std::make_shared<CameraParams::FPSCameraConfig>();
  config->m_Axis = glm::vec3(0.0f, 1.0f, 0.0f);
  config->m_Angle = 1;
  m_Impl->m_Camera->rotate(config);
}