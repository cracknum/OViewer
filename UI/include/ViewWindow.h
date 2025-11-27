#ifndef VIEW_WINDOW_H
#define VIEW_WINDOW_H
#include "ViewWindowBase.h"
#include <memory>

class ViewWindowConfig;
class QOpenGLFunctions_4_4_Core;
class InteractiveObject;
class ViewWindow final : public ViewWindowBase
{
  Q_OBJECT

public:
  explicit ViewWindow(
    QOpenGLFunctions_4_4_Core* function, const ViewWindowConfig& config, QObject* parent = nullptr);
  void drawWindow() override;

  private slots:
  void rotateCamera();
private:
  struct Impl;
  std::unique_ptr<Impl> m_Impl;
};
#endif