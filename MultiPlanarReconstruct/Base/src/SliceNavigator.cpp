#include "SliceNavigator.h"

#include "PlaneGeometry.h"
#include "SlicedGeometry.h"

#include <spdlog/spdlog.h>
#include <valarray>
#include <vtkImageData.h>
#include <vtkMath.h>
#include <vtkObjectFactory.h>

vtkStandardNewMacro(SliceNavigator);

PlaneGeometry* SliceNavigator::getCurrentPlaneGeometry() const
{
  if (!this->mSlicedGeometry)
  {
    SPDLOG_ERROR("sliced geometry is nullptr, please using setReferenceImage method first");
    return nullptr;
  }

  return this->mSlicedGeometry->getPlaneGeometry(CurrentSliceNumber);
}

void SliceNavigator::setCurrentSlicePosition(int sliceNumber)
{
  if (!this->mSlicedGeometry)
  {
    SPDLOG_ERROR("sliced geometry is nullptr, please using setReferenceImage method first");
    return;
  }
  this->mSlicedGeometry->setPlaneGeometry(sliceNumber);
  CurrentSliceNumber = sliceNumber;
}
void SliceNavigator::sliceToCenter() {}
void SliceNavigator::nextSlice()
{
  if (this->mSlicedGeometry->setPlaneGeometry(CurrentSliceNumber + 1))
  {
    CurrentSliceNumber += 1;
    this->Modified();
  }
}
void SliceNavigator::previousSlice()
{
  if (this->mSlicedGeometry->setPlaneGeometry(CurrentSliceNumber - 1))
  {
    CurrentSliceNumber -= 1;
    this->Modified();
  }
}

SliceNavigator::SliceNavigator()
  : PlaneType(StandardPlane::None)
  , CurrentSliceNumber(0)
{
}

SliceNavigator::~SliceNavigator() {}

void SliceNavigator::setReferenceImage(vtkImageData* imageData, StandardPlane planeType)
{
  if (!this->mSlicedGeometry)
  {
    this->mSlicedGeometry = vtkSmartPointer<SlicedGeometry>::New();
  }

  if (!imageData)
  {
    SPDLOG_ERROR("reference image is nullptr");
    return;
  }

  this->mSlicedGeometry->initialize(imageData, planeType);
  this->PlaneType = planeType;
}
