#include "SliceNavigator.h"
#include <vtkObjectFactory.h>

vtkStandardNewMacro(SliceNavigator);

PlaneGeometry* SliceNavigator::getCurrentPlaneGeometry() const
{
  return nullptr;
}

void SliceNavigator::setCurrentSlicePosition(int sliceNumber) {}

SliceNavigator::SliceNavigator()
  : PlaneType(StandardPlane::None)
  , CurrentSliceNumber(0)
{
}

SliceNavigator::~SliceNavigator() {}
