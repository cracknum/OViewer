#include "MPRSliceRepresentation.h"
#include "PlaneGeometry.h"
#include "SlicedGeometry.h"
#include "SliceNavigator.h"
#include <spdlog/spdlog.h>
#include <vtkActor.h>
#include <vtkImageData.h>
#include <vtkImageMapToColors.h>
#include <vtkImageReslice.h>
#include <vtkMatrix4x4.h>
#include <vtkMetaImageWriter.h>
#include <vtkObjectFactory.h>
#include <vtkPlaneSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkPropCollection.h>
#include <vtkTexture.h>
#include <vtkTransform.h>
#include <vtkWindowLevelLookupTable.h>

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
  this->mLevel = level;
  this->mWindow = window;
  this->Modified();
}
SliceNavigator* MPRSliceRepresentation::getSliceNavigator()
{
  return mSlicedNavigator;
}
void MPRSliceRepresentation::BuildRepresentation()
{
  if (this->mPlaneType == StandardPlane::None || !this->mImageData || !this->mWindow)
  {
    SPDLOG_ERROR("plane type, image data or slice window is not setted");
    return;
  }
  if (this->mSlicedNavigator->GetPlaneType() == StandardPlane::None)
  {
    this->mSlicedNavigator->setReferenceImage(this->mImageData, this->mPlaneType);
  }
  auto planeGeometry = this->mSlicedNavigator->getCurrentPlaneGeometry();
  double bounds[6]{};
  double spacing[3]{};
  double origin[3]{};
  planeGeometry->getOrigin(origin);
  planeGeometry->getBounds(bounds);
  planeGeometry->getSpacing(spacing);
  auto transform = planeGeometry->getIndexToWorldTransform();
  auto matrix = transform->GetMatrix();
  this->mResliceAxes->DeepCopy(matrix);
  mReslicer->SetInputData(mImageData);
  mReslicer->SetOutputExtent(bounds[0], bounds[1], bounds[2], bounds[3], 0, 0);
  mReslicer->SetOutputSpacing(spacing[0], spacing[1], spacing[2]);
  mReslicer->SetOutputOrigin(origin);
  mReslicer->Update();
  auto sliceImage = mReslicer->GetOutput();
  auto metaDataWriter = vtkSmartPointer<vtkMetaImageWriter>::New();
  metaDataWriter->SetInputData(sliceImage);
  auto sliceNumber = this->mSlicedNavigator->GetCurrentSliceNumber();
  std::string fileName = std::to_string(sliceNumber) + "_slice.mhd";
  metaDataWriter->SetFileName(fileName.c_str());
  metaDataWriter->Write();
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

void MPRSliceRepresentation::GetActors(vtkPropCollection* props)
{
  props->AddItem(this->mTexturedActor);
}

int MPRSliceRepresentation::RenderOpaqueGeometry(vtkViewport* vtkNotUsed(viewport))
{
  BuildRepresentation();
  return 1;
}
