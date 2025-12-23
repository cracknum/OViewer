#include "PlaneGeometry.h"
#include "SlicedGeometry.h"
#include <spdlog/spdlog.h>
#include <vector>
#include <vtkImageData.h>
#include <vtkMath.h>
#include <vtkMatrix3x3.h>
#include <vtkMatrix4x4.h>
#include <vtkObjectFactory.h>
#include <vtkSmartPointer.h>
#include <vtkTransform.h>

vtkStandardNewMacro(SlicedGeometry);

struct SlicedGeometry::Private
{
  std::vector<vtkSmartPointer<PlaneGeometry>> mPlaneGeometryVector;
  vtkSmartPointer<vtkImageData> mReferenceData;
  double mDirectionVector[3];
  StandardPlane mPlaneType;
  int mNumberOfSlices;

  Private()
    : mPlaneType(StandardPlane::None)
    , mNumberOfSlices(0)
    , mDirectionVector{}
  {
  }
};

void SlicedGeometry::initialize(vtkImageData* imageData, StandardPlane planeType)
{
  if (!imageData)
  {
    return;
  }
  mPrivate->mPlaneType = planeType;
  mPrivate->mReferenceData = imageData;

  auto standardPlaneGeometry = initializeStandardPlanes();
  if (!standardPlaneGeometry)
  {
    SPDLOG_ERROR("no reference image data setted");
    return;
  }

  auto imageDataDirectionMatrix = imageData->GetDirectionMatrix();

  PlaneGeometry::normalizeMatrixColumn(imageDataDirectionMatrix);
  auto inverseMatrix = vtkSmartPointer<vtkMatrix3x3>::New();
  vtkMatrix3x3::Transpose(imageDataDirectionMatrix, inverseMatrix);

  auto axes = PlaneGeometry::calculateDominantAxis(inverseMatrix);

  auto worldAxis = -1; // normal direction
  switch (planeType)
  {
    case StandardPlane::Axial:
      worldAxis = 2;
      break;
    case StandardPlane::Coronal:
      worldAxis = 1;
      break;
    case StandardPlane::Sagittal:
      worldAxis = 0;
      break;
    default:
      SPDLOG_ERROR("plane type setting error");
      return;
      break;
  }
  auto dominantAxis = axes[worldAxis];
  auto viewSpacing = mPrivate->mReferenceData->GetSpacing()[dominantAxis];
  mPrivate->mNumberOfSlices = mPrivate->mReferenceData->GetDimensions()[dominantAxis];

  double bounds[6];
  standardPlaneGeometry->getBounds(bounds);
  bounds[4] = 0;
  bounds[5] = mPrivate->mNumberOfSlices - 1;

  mPrivate->mPlaneGeometryVector.assign(mPrivate->mNumberOfSlices, nullptr);
  mPrivate->mPlaneGeometryVector[0] = standardPlaneGeometry;

  auto directionVector = standardPlaneGeometry->getAxisVector(Axis::Z);

  // clang-format off
  double spacing[3] = { 
    standardPlaneGeometry->getExtentInMM(Axis::X) / standardPlaneGeometry->getExtent(Axis::X),
    standardPlaneGeometry->getExtentInMM(Axis::Y) / standardPlaneGeometry->getExtent(Axis::Y),
    standardPlaneGeometry->getExtentInMM(Axis::Z) / standardPlaneGeometry->getExtent(Axis::Z) 
  };
  // clang-format on

  directionVector.Normalize();
  vtkMath::MultiplyScalar(directionVector.GetData(), spacing[2]);

  auto indexToWorldTransform = vtkSmartPointer<vtkTransform>::New();
  indexToWorldTransform->DeepCopy(standardPlaneGeometry->getIndexToWorldTransform());

  Superclass::setIndexToWorldTransform(indexToWorldTransform);
  Superclass::setBounds(bounds);
  Superclass::setSpacing(spacing);
  std::copy(directionVector.GetData(), directionVector.GetData() + 3, mPrivate->mDirectionVector);

  this->Modified();
}

void SlicedGeometry::reinitialize() {}

PlaneGeometry* SlicedGeometry::getPlaneGeometry(int sliceNumber) const
{
  if (!isValidSlice(sliceNumber))
  {
    return nullptr;
  }

  return mPrivate->mPlaneGeometryVector[sliceNumber];
}

unsigned int SlicedGeometry::getNumberOfSlices() const
{
  return mPrivate->mNumberOfSlices;
}

bool SlicedGeometry::isValidSlice(int sliceNumber) const
{
  if (sliceNumber > mPrivate->mPlaneGeometryVector.size() ||
    !mPrivate->mPlaneGeometryVector[sliceNumber])
  {
    return false;
  }
  return true;
}

SlicedGeometry::SlicedGeometry()
{
  mPrivate = std::make_unique<Private>();
  initialize(nullptr, StandardPlane::None);
}

SlicedGeometry::~SlicedGeometry() = default;

double SlicedGeometry::calculateSpacing(const double spacing[3], const vtkVector3d& direction)
{
  return 0.0;
}

void SlicedGeometry::reinitializePlanes() {}

vtkSmartPointer<PlaneGeometry> SlicedGeometry::initializeStandardPlanes()
{
  // initialize the planes' geometry
  if (!mPrivate->mReferenceData)
  {
    return vtkSmartPointer<PlaneGeometry>();
  }

  vtkSmartPointer<PlaneGeometry> planeGeometry = vtkSmartPointer<PlaneGeometry>::New();
  planeGeometry->initializeStandardPlane(mPrivate->mReferenceData, mPrivate->mPlaneType);

  return planeGeometry;
}
