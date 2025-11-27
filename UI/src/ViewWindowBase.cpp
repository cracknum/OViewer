#include "ViewWindowBase.h"

ViewWindowBase::ViewWindowBase(
  QOpenGLFunctions_4_4_Core* function, const ViewWindowConfig& config, QObject* parent)
  : m_Function(function)
  , m_Config(config)
  , QObject(parent)
{
}

void ViewWindowBase::update()
{
  drawWindow();
}

ViewWindowConfig::IdType ViewWindowBase::getWindowId() const
{
  return m_Config.m_Id;
}

void ViewWindowBase::updateViewSize(const ViewWindowConfig::WindowSize& parentWindowSize)
{
  m_Config.m_ViewPortSize[0] = (static_cast<int>(m_Config.m_ViewPort.x() * parentWindowSize[0]));
  m_Config.m_ViewPortSize[1] = (static_cast<int>(m_Config.m_ViewPort.y() * parentWindowSize[1]));
  m_Config.m_ViewPortSize[2] = static_cast<int>(m_Config.m_ViewPort.width() * parentWindowSize[0]);
  m_Config.m_ViewPortSize[3] = static_cast<int>(m_Config.m_ViewPort.height() * parentWindowSize[1]);
}

void ViewWindowBase::addPrimitive(std::shared_ptr<InteractiveObject> primitive)
{
  m_Primitives.push_back(primitive);
}
