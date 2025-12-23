#ifndef MPR_SLICE_WIDGET_H
#define MPR_SLICE_WIDGET_H

#include <vtkAbstractWidget.h>

class MPRSliceWidget final : public vtkAbstractWidget
{
public:
  static MPRSliceWidget* New();
  vtkTypeMacro(MPRSliceWidget, vtkAbstractWidget);
  void CreateDefaultRepresentation() override;

protected:
  MPRSliceWidget();
  ~MPRSliceWidget() override;
};

#endif // MPR_SLICE_WIDGET_H
