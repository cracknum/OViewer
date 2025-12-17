#include "BaseGeometry.h"
#include <vtkBoundingBox.h>
#include <vtkMatrix4x4.h>
#include <vtkObjectFactory.h>
#include <vtkSmartPointer.h>
#include <vtkTransform.h>

struct BaseGeometry::Private
{
	// index coordinates bounding box;
  std::unique_ptr<vtkBoundingBox> mBoundingBox;
  vtkSmartPointer<vtkTransform> mIndexToWorldTransform;

  Private()
  {
    mBoundingBox = std::make_unique<vtkBoundingBox>();
    mIndexToWorldTransform = vtkSmartPointer<vtkTransform>::New();
    mIndexToWorldTransform->Identity();
  }
};

vtkStandardNewMacro(BaseGeometry)

  BaseGeometry::BaseGeometry()
{
  mPrivate = std::make_unique<Private>();
}

BaseGeometry::~BaseGeometry() {}

void BaseGeometry::getOrigin(double origin[3]) const
{
  auto matrix = mPrivate->mIndexToWorldTransform->GetMatrix();
  for (size_t i = 0; i < 3; i++)
  {
    origin[i] = matrix->GetElement(i, 3);
  }
}

void BaseGeometry::setOrigin(double origin[3])
{
  auto matrix = mPrivate->mIndexToWorldTransform->GetMatrix();
  for (size_t i = 0; i < 3; i++)
  {
    matrix->SetElement(i, 3, origin[i]);
  }
  Modified();
}

void BaseGeometry::getSpacing(double spacing[3]) const
{
  spacing[0] = getSpacing(Axis::X);
  spacing[1] = getSpacing(Axis::Y);
  spacing[2] = getSpacing(Axis::Z);
}

double BaseGeometry::getSpacing(Axis axis) const
{
  auto matrix = mPrivate->mIndexToWorldTransform->GetMatrix();
  auto xAxis = vtkVector3d(
    matrix->GetElement(0, axis), matrix->GetElement(1, axis), matrix->GetElement(2, axis));

  return xAxis.Norm();
}

void BaseGeometry::setSpacing(double spacing[3])
{
  auto matrix = mPrivate->mIndexToWorldTransform->GetMatrix();
  for (size_t axis = Axis::X; axis <= Axis::Z; axis++)
  {
    auto vector = vtkVector3d(
      matrix->GetElement(0, axis), matrix->GetElement(1, axis), matrix->GetElement(2, axis));
    vector.Normalize();
    matrix->SetElement(0, axis, vector.GetX() * spacing[axis]);
    matrix->SetElement(1, axis, vector.GetY() * spacing[axis]);
    matrix->SetElement(2, axis, vector.GetZ() * spacing[axis]);
  }
  Modified();
}

vtkTransform* BaseGeometry::getIndexToWorldTransform()
{
  return mPrivate->mIndexToWorldTransform;
}

vtkTransform* BaseGeometry::getWorldToIndexTransform()
{
  return vtkTransform::SafeDownCast(mPrivate->mIndexToWorldTransform->GetInverse());
}

void BaseGeometry::setIndexToWorldTransform(vtkTransform* transform)
{
  if (transform)
  {
    mPrivate->mIndexToWorldTransform->DeepCopy(transform);
	Modified();
  }
}

void BaseGeometry::getBounds(double bounds[6]) const
{
  mPrivate->mBoundingBox->GetBounds(bounds);
}

void BaseGeometry::setBounds(double bounds[6])
{
  mPrivate->mBoundingBox->SetBounds(bounds);
  Modified();
}

double BaseGeometry::getExtent(Axis axis) const
{
  return mPrivate->mBoundingBox->GetLength(axis);
}

double BaseGeometry::getExtentInMM(Axis axis) const
{
  return getSpacing(axis) * getExtent(axis);
}

void BaseGeometry::getCenter(double center[3]) const
{
  mPrivate->mBoundingBox->GetCenter(center);

  mPrivate->mIndexToWorldTransform->TransformPoint(center, center);
}

vtkVector3d BaseGeometry::getAxisVector(Axis axis) const
{
  auto matrix = mPrivate->mIndexToWorldTransform->GetMatrix();
  auto vector = vtkVector3d(
    matrix->GetElement(0, axis), matrix->GetElement(1, axis), matrix->GetElement(2, axis));
  auto axisIndexLength = getExtent(axis);
  vector.SetX(vector.GetX() * axisIndexLength);
  vector.SetY(vector.GetY() * axisIndexLength);
  vector.SetZ(vector.GetZ() * axisIndexLength);
  return vector;
}
