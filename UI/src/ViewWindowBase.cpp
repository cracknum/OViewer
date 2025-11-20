#include "ViewWindowBase.h"

ViewWindowBase::ViewWindowBase(QOpenGLContext* context, const ViewWindowConfig& config, QObject* parent)
  : m_Context(context)
  , m_Config(config)
  , QObject(parent)
{
}

void ViewWindowBase::update()
{
  drawWindow();
}
