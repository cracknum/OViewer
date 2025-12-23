#include "MPRSliceWidget.h"
#include "MPRSliceRepresentation.h"
#include <vtkObjectFactory.h>

vtkStandardNewMacro(MPRSliceWidget);

void MPRSliceWidget::CreateDefaultRepresentation()
{
  if (!this->WidgetRep)
  {
    this->WidgetRep = vtkSmartPointer<MPRSliceRepresentation>::New();
  }
}
MPRSliceWidget::MPRSliceWidget() {}
MPRSliceWidget::~MPRSliceWidget() {}