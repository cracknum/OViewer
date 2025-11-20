#ifndef VIEW_WINDOW_H
#define VIEW_WINDOW_H
#include "ViewWindowBase.h"

class ViewWindowConfig;
class QOpenGLContext;
class ViewWindow final : public ViewWindowBase
{
  Q_OBJECT

public:
  explicit ViewWindow(QOpenGLContext* context, const ViewWindowConfig& config, QObject* parent = nullptr);
  void drawWindow() override;
};
#endif