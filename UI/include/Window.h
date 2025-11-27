#ifndef WINDOW_H
#define WINDOW_H
#include "Macros.h"
#include "UIExport.h"
#include <memory>
#include <QVTKOpenGLNativeWidget.h>
class vtkFloatArray;
class vtkActor2D;
class ViewWindow;

class UI_API Window final : public QVTKOpenGLNativeWidget
{
  Q_OBJECT
public:
  explicit Window(QWidget* parent = nullptr);

  ~Window() override;

  void addViewWindow(int viewId, vtkFloatArray* windowRange, vtkFloatArray* background);
  void addSplitLine(vtkFloatArray* colorArray, vtkFloatArray* linePoints);
  [[nodiscard]] ViewWindow* getViewWindow(int viewId) const;

protected:
  vtkSmartPointer<vtkActor2D> createLineActor(vtkFloatArray* colorArray, vtkFloatArray* linePoints);
  void initSplitLine();
  void initViewWindows();

private:
  struct Impl;
  std::unique_ptr<Impl> m_Impl;
};

#endif // WINDOW_H
