#ifndef MPR_SLICE_WIDGET_H
#define MPR_SLICE_WIDGET_H

#include "PlaneType.h"
#include "RenderingExport.h"
#include <vtkAbstractWidget.h>
#include <vtkSmartPointer.h>

class vtkImageData;

class RENDERING_API MPRSliceWidget final : public vtkAbstractWidget
{
public:
  static MPRSliceWidget* New();
  vtkTypeMacro(MPRSliceWidget, vtkAbstractWidget);
  void CreateDefaultRepresentation() override;
  void setLevel(int level);
  void setWindow(int window);
  int getLevel();
  int getWindow();
  void setImageData(vtkImageData* imageData);
  void setSliceType(StandardPlane sliceType);

protected:
  MPRSliceWidget();
  ~MPRSliceWidget() override;

  static void sliceForward(vtkAbstractWidget*);
  static void sliceBackward(vtkAbstractWidget*);

private:
  int mLevel;
  int mWindow;
  StandardPlane mPlaneType;
  vtkSmartPointer<vtkImageData> mImageData;
};

#endif // MPR_SLICE_WIDGET_H
