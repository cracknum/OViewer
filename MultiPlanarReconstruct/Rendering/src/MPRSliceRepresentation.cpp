#include "MPRSliceRepresentation.h"
#include "PlaneGeometry.h"
#include "SliceNavigator.h"
#include "SlicedGeometry.h"
#include <vtkActor.h>
#include <vtkImageMapToColors.h>
#include <vtkImageReslice.h>
#include <vtkMatrix4x4.h>
#include <vtkObjectFactory.h>
#include <vtkPlaneSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkTexture.h>
#include <vtkWindowLevelLookupTable.h>
#include <vtkImageData.h>


vtkStandardNewMacro(MPRSliceRepresentation);

void MPRSliceRepresentation::PrintSelf(ostream& os, vtkIndent indent) {}

void MPRSliceRepresentation::setImageData(vtkImageData* imageData)
{
  if (this->mImageData != imageData)
  {
    this->mImageData = imageData;
    this->Modified();
  }
}

void MPRSliceRepresentation::setSliceType(StandardPlane planeType)
{
  if (this->mPlaneType != planeType)
  {
    this->mPlaneType = planeType;
    this->Modified();
  }
}

void MPRSliceRepresentation::setLevelWindow(int level, int window)
{
  auto lookupTable = vtkWindowLevelLookupTable::SafeDownCast(this->mLookupTable);
  lookupTable->SetWindow(window);
  lookupTable->SetLevel(level);
  lookupTable->Build();
  this->Modified();
}
void MPRSliceRepresentation::BuildRepresentation()
{
  updateInternal();
}
MPRSliceRepresentation::MPRSliceRepresentation()
  : mLevel(0)
  , mWindow(0)
  , mPlaneType(StandardPlane::None)
{
  // 构建切片渲染管线
  this->mTexture = vtkSmartPointer<vtkTexture>::New();
  this->mLookupTable = vtkSmartPointer<vtkLookupTable>::New();
  this->mColorMap = vtkSmartPointer<vtkImageMapToColors>::New();
  this->mColorMap->SetLookupTable(this->mLookupTable);
  this->mTexture->SetInputConnection(this->mColorMap->GetOutputPort());

  this->mResliceAxes = vtkSmartPointer<vtkMatrix4x4>::New();
  this->mResliceAxes->Identity();
  this->mSlicedNavigator = vtkSmartPointer<SliceNavigator>::New();

  auto reslicer = vtkSmartPointer<vtkImageReslice>::New();
  reslicer->SetBackgroundColor(0.0, 0.0, 0.0, 1.0);
  reslicer->SetAutoCropOutput(0);
  reslicer->SetBackgroundLevel(1);
  reslicer->SetBorder(0);
  reslicer->SetEnableSMP(1);
  reslicer->SetInterpolationModeToLinear();
  this->mReslicer = reslicer;
  this->mColorMap->SetInputConnection(this->mReslicer->GetOutputPort());

  this->mPlaneSource = vtkSmartPointer<vtkPlaneSource>::New();
  this->mTexturedMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  this->mTexturedActor = vtkSmartPointer<vtkActor>::New();

  this->mTexturedMapper->SetInputConnection(this->mPlaneSource->GetOutputPort());
  this->mTexturedActor->SetMapper(this->mTexturedMapper);
  this->mTexturedActor->SetTexture(this->mTexture);
}
MPRSliceRepresentation::~MPRSliceRepresentation() = default;

void MPRSliceRepresentation::updateInternal() {}
