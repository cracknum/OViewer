#include "ViewWindowBase.h"

ViewWindowBase::ViewWindowBase(QOpenGLFunctions_4_4_Core* function, const ViewWindowConfig& config, QObject* parent)
  : m_Function(function)
  , m_Config(config)
  , QObject(parent)
{
}

void ViewWindowBase::update()
{
  drawWindow();
}
