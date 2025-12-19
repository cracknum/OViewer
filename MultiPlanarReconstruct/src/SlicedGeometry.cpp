#include "SlicedGeometry.h"
#include "PlaneGeometry.h"
#include <vector>
#include <vtkImageData.h>
#include <vtkMatrix3x3.h>
#include <vtkMatrix4x4.h>
#include <vtkObjectFactory.h>
#include <vtkSmartPointer.h>
#include <vtkTransform.h>

vtkStandardNewMacro(SlicedGeometry)

  struct SlicedGeometry::Private
{
  std::vector<vtkSmartPointer<PlaneGeometry>> mPlaneGeometryVector;
};

void SlicedGeometry::initialize(vtkImageData* imageData)
{
  if (!imageData)
  {
    return;
  }

  double origin[3];
  imageData->GetOrigin(origin);
  double spacing[3];
  imageData->GetSpacing(spacing);

  auto affineMatrix = imageData->GetDirectionMatrix();
  vtkSmartPointer<vtkMatrix4x4> indexToWorldMatrix = vtkSmartPointer<vtkMatrix4x4>::New();
  indexToWorldMatrix->Identity();

  // construct affine matrix;
  for (size_t i = 0; i < 3; i++)
  {
    for (size_t j = 0; j < 3; i++)
    {
      auto elem = affineMatrix->GetElement(i, j);
      if (i == j)
      {
        elem *= spacing[i];
      }
      indexToWorldMatrix->SetElement(i, j, elem);
    }
  }

  // set offset
  for (size_t i = 0; i < 3; i++)
  {
    indexToWorldMatrix->SetElement(i, 3, origin[i]);
  }
  vtkSmartPointer<vtkTransform> indexToWorldTransform = vtkSmartPointer<vtkTransform>::New();
  indexToWorldTransform->SetMatrix(indexToWorldMatrix);

  Superclass::setIndexToWorldTransform(indexToWorldTransform);
  Superclass::setImageGeometry(true);

  int bounds[6];
  imageData->GetExtent(bounds);
  double dBounds[6] = {bounds[0], bounds[1], bounds[2], bounds[3], bounds[4], bounds[5]};
  Superclass::setBounds(dBounds);

  initializePlanes();
}

void SlicedGeometry::reinitialize() {}

PlaneGeometry* SlicedGeometry::getPlaneGeometry(int sliceNumber) const
{
  return nullptr;
}

unsigned int SlicedGeometry::getNumberOfSlices() const
{
  return 0;
}

bool SlicedGeometry::isValidSlice(int sliceNumber) const
{
  return false;
}

SlicedGeometry::SlicedGeometry()
{
  initialize(nullptr);
}

SlicedGeometry::~SlicedGeometry() = default;

double SlicedGeometry::calculateSpacing(const double spacing[3], const vtkVector3d& direction)
{
  return 0.0;
}

void SlicedGeometry::reinitializePlanes() {}

void SlicedGeometry::initializePlanes(const double planeNormal[3]) {
	// initialize the planes' geometry

	vtkSmartPointer<PlaneGeometry> planeGeometry = vtkSmartPointer<PlaneGeometry>::New();
	planeGeometry->initializePlane()
}
