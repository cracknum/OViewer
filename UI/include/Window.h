#ifndef WINDOW_H
#define WINDOW_H
#include <QWidget>
#include "UIExport.h"
class UI_API Window final : public QWidget
{
  Q_OBJECT
public:
  explicit Window(QWidget* parent = nullptr);
  ~Window() override;

protected:
  void mousePressEvent(QMouseEvent* event) override;
  void mouseReleaseEvent(QMouseEvent* event) override;
  void mouseDoubleClickEvent(QMouseEvent* event) override;
  void mouseMoveEvent(QMouseEvent* event) override;
  void wheelEvent(QWheelEvent* event) override;
  void keyPressEvent(QKeyEvent* event) override;
  void keyReleaseEvent(QKeyEvent* event) override;
  void enterEvent(QEvent* event) override;
  void leaveEvent(QEvent* event) override;
  void resizeEvent(QResizeEvent* event) override;
};


#endif // WINDOW_H
