#ifndef RENDER_WINDOW_H
#define RENDER_WINDOW_H
#include "UIExport.h"
#include <QVTKOpenGLNativeWidget.h>
#include <vtkSmartPointer.h>


class vtkTransform;
class vtkPlaneSource;
class vtkActor;

class UI_API RenderWindow : public QVTKOpenGLNativeWidget
{
  Q_OBJECT;

public:
  explicit RenderWindow(QWidget* parent = nullptr);
  ~RenderWindow() override;

protected:
  vtkSmartPointer<vtkTransform> mPlaneIndexToWorldTransform;
  vtkSmartPointer<vtkPlaneSource> mPlaneSource;
  vtkSmartPointer<vtkActor> mPlaneActor;
};
#endif // RENDER_WINDOW_H