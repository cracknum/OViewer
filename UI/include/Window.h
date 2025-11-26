#ifndef WINDOW_H
#define WINDOW_H
#include "Macros.h"
#include "UIExport.h"
#include <QVTKOpenGLNativeWidget.h>

class UI_API Window final : public QVTKOpenGLNativeWidget
{
  Q_OBJECT
public:
  explicit Window(QWidget* parent = nullptr);
  ~Window() override;

private:
};

#endif // WINDOW_H
