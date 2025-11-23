#ifndef WINDOW_H
#define WINDOW_H
#include "UIExport.h"
#include <QOpenGLFunctions_4_4_Core>
#include <QOpenGLWidget>
#include <QWidget>
#include <memory>
#include "Macros.h"

class ViewWindowBase;
struct ViewWindowConfig;

class UI_API Window final
  : public QOpenGLWidget
  , public QOpenGLFunctions_4_4_Core
{
  Q_OBJECT
public:
  explicit Window(QWidget* parent = nullptr);
  ~Window() override;
  DISALLOW_COPY_AND_MOVE(Window);

  using ViewWindowVector = std::vector<ViewWindowBase*>;

  void addViewWindow(const ViewWindowConfig& config);
protected:
  void initializeGL() override;
  void resizeGL(int w, int h) override;
  void paintGL() override;
  void mousePressEvent(QMouseEvent* event) override;
  void mouseReleaseEvent(QMouseEvent* event) override;
  void mouseDoubleClickEvent(QMouseEvent* event) override;
  void mouseMoveEvent(QMouseEvent* event) override;
  void wheelEvent(QWheelEvent* event) override;
  void keyPressEvent(QKeyEvent* event) override;
  void keyReleaseEvent(QKeyEvent* event) override;

private:
  struct Impl;
  std::unique_ptr<Impl> m_Impl;
};

#endif // WINDOW_H
