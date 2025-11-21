#ifndef VIEW_WINDOW_H
#define VIEW_WINDOW_H
#include "ViewWindowBase.h"

class ViewWindowConfig;
class QOpenGLFunctions_4_4_Core;
class ViewWindow final : public ViewWindowBase
{
  Q_OBJECT

public:
  explicit ViewWindow(QOpenGLFunctions_4_4_Core* function, const ViewWindowConfig& config, QObject* parent = nullptr);
  void drawWindow() override;
};
#endif