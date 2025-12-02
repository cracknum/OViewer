#include "ViewWindow.h"
#include "CameraConfig.h"
#include "FPSCamera.h"
#include "InteractiveObject.h"
#include "OrbitCamera.h"
#include "TrackballCamera.h"
#include <QOpenGLFunctions_4_4_Core>
#include <spdlog/spdlog.h>

#define DEBUG_CAMERA ;
#if defined(DEBUG_CAMERA)
#define TRACK_BALL_CAMERA_DEBUG
#include <QTimer>

#if defined(FPS_CAMERA_DEBUG)
using CameraConfig = CameraParams::FPSCameraConfig;
#elif defined(TRACK_BALL_CAMERA_DEBUG)
using CameraConfig = CameraParams::TrackballCameraConfig;
using CameraClass = TrackBallCamera;
#endif
#endif

struct ViewWindow::Impl
{
  std::unique_ptr<AbstractCamera> m_Camera;
#if defined(DEBUG_CAMERA)
  QTimer timer;
  std::shared_ptr<CameraConfig> cameraConfig;
#endif
  Impl()
  {
#if defined(DEBUG_CAMERA)
#if defined(FPS_CAMERA_DEBUG)
    using CameraConfig = CameraParams::FPSCameraConfig;
#elif defined(TRACK_BALL_CAMERA_DEBUG)
    using CameraConfig = CameraParams::TrackballCameraConfig;
    using CameraClass = TrackBallCamera;
#endif

    cameraConfig = std::make_shared<CameraConfig>();
    std::shared_ptr<CameraParams::OrthogonalConfig> projectConfig =
      std::make_shared<CameraParams::OrthogonalConfig>();

#if defined(ORBIT_CAMERA_DEBUG)
    cameraConfig->m_Target = glm::vec3(0, 0, -200.0f);
    cameraConfig->m_Distance = 10.0f;
    cameraConfig->m_Yaw = 1;
#elif defined(FPS_CAMERA_DEBUG)
    cameraConfig->m_Axis = glm::vec3(0.0f, 1.0f, 0.0f);
#elif defined(TRACK_BALL_CAMERA_DEBUG)
    cameraConfig->m_Distance = 1.0f;
    cameraConfig->m_Target = glm::vec3(0.0f, 0.0f, -200.0f);
#endif
    m_Camera = std::make_unique<CameraClass>(cameraConfig, projectConfig);

#endif
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
#if defined(ORBIT_CAMERA_DEBUG)
  static float yawDelta = 0.0f;
  static float pitchDelta = 0.0f;
  yawDelta += 1.0f;
  pitchDelta += 0.5f;
  m_Impl->cameraConfig->m_Yaw = yawDelta;
  m_Impl->cameraConfig->m_Pitch = pitchDelta;
#elif defined(FPS_CAMERA_DEBUG)
  m_Impl->cameraConfig->m_Angle = 1;
#elif defined(TRACK_BALL_CAMERA_DEBUG)
  if (std::abs(m_Impl->cameraConfig->m_CurrentMousePos.y - 0.5f) < 1e-6)
  {
    m_Impl->cameraConfig->m_PreviousMousePos = glm::vec3(0.0f, 0.0f, 0.0f);
    m_Impl->cameraConfig->m_CurrentMousePos.y = -.5f;
  }

  m_Impl->cameraConfig->m_CurrentMousePos.y += 0.1;
  m_Impl->cameraConfig->m_CurrentMousePos.y =
    glm::clamp(m_Impl->cameraConfig->m_CurrentMousePos.y, -0.5f, 0.5f);
#endif
  m_Impl->m_Camera->rotate(m_Impl->cameraConfig);
}
