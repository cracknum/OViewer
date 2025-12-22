#include "PlaneGeometry.h"
#include <spdlog/spdlog.h>
#include <vtkCommand.h>
#include <vtkImageData.h>
#include <vtkMath.h>
#include <vtkMatrix3x3.h>
#include <vtkObjectFactory.h>
#include <vtkTransform.h>

vtkStandardNewMacro(PlaneGeometry);

void PlaneGeometry::initializePlane(double bounds[4], vtkTransform* indexToWorldTransform)
{
  double bounds3D[6] = { bounds[0], bounds[1], bounds[2], bounds[4], 0, 0 };
  Superclass::setBounds(bounds);
  Superclass::setIndexToWorldTransform(indexToWorldTransform);
}

void PlaneGeometry::initializePlane(vtkImageData* imageData, const double planeNormal[3]) {}

void PlaneGeometry::initializeStandardPlane(vtkImageData* imageData, StandardPlane planeType)
{
  // WARN: the matrix must be a perpendicular matrix, otherwise, this will be a fault
  int extents[3]{};
  double spacing[3]{};
  double imageOrigin[3]{};
  imageData->GetOrigin(imageOrigin);

  auto imageDataDirectionMatrix = imageData->GetDirectionMatrix();
  imageData->GetSpacing(spacing);

  auto scaleMatrix = vtkSmartPointer<vtkMatrix3x3>::New();
  auto matrix = vtkSmartPointer<vtkMatrix3x3>::New();
  scaleMatrix->Identity();
  scaleMatrix->SetElement(0, 0, spacing[0]);
  scaleMatrix->SetElement(1, 1, spacing[1]);
  scaleMatrix->SetElement(2, 2, spacing[2]);
  vtkMatrix3x3::Multiply3x3(imageDataDirectionMatrix, scaleMatrix, matrix);
  auto indexToWorldMatrix = vtkSmartPointer<vtkMatrix4x4>::New();
  for (size_t i = 0; i < 3; i++)
  {
    for (size_t j = 0; j < 3; j++)
    {
      indexToWorldMatrix->SetElement(i, j, matrix->GetElement(i, j));
    }
  }
  indexToWorldMatrix->SetElement(0, 3, imageOrigin[0]);
  indexToWorldMatrix->SetElement(1, 3, imageOrigin[1]);
  indexToWorldMatrix->SetElement(2, 3, imageOrigin[2]);

  auto worldToIndexMatrix = vtkSmartPointer<vtkMatrix3x3>::New();
  auto normalizedMatrix = vtkSmartPointer<vtkMatrix3x3>::New();
  normalizedMatrix->DeepCopy(matrix);
  // normalize column vector
  normalizeMatrixColumn(normalizedMatrix);

  vtkMatrix3x3::Transpose(normalizedMatrix, worldToIndexMatrix);

  auto axes = calculateDominantAxis(worldToIndexMatrix); // axes: world axis -> image axis

  int directionSign[3];

  // reconstruct the relationship between image axis and world axis
  for (size_t i = 0; i < 3; i++)
  {
    int dominantAxis = axes[i];
    // 因为这个值为主轴中的最大值，所以可以代表主轴的方向
    directionSign[i] = worldToIndexMatrix->GetElement(dominantAxis, i) > 0 ? 1 : -1;
    extents[i] = imageData->GetDimensions()[dominantAxis];
    spacing[i] = imageData->GetSpacing()[dominantAxis];
  }

  // 根据最新的轴和轴的方向调整矩阵
  for (size_t i = 0; i < 3; i++)
  {
    for (size_t j = 0; j < 3; j++)
    {
      auto elem = worldToIndexMatrix->GetElement(axes[j], i);
      elem *= directionSign[j] * spacing[j];
      indexToWorldMatrix->SetElement(i, j, elem);
    }
  }

  // 计算平面的偏移到起始点的偏移量
  for (size_t i = 0; i < 3; i++)
  {
    double offset = directionSign[i] > 0 ? 0 : extents[i];
    offset += directionSign[i] * 0.5; // 移动到角点

    for (size_t j = 0; j < 3; j++)
    {
      imageOrigin[j] -= offset * matrix->GetElement(j, i);
    }
  }

  indexToWorldMatrix->SetElement(0, 3, imageOrigin[0]);
  indexToWorldMatrix->SetElement(1, 3, imageOrigin[1]);
  indexToWorldMatrix->SetElement(2, 3, imageOrigin[2]);

  auto indexToWorldTransform = vtkSmartPointer<vtkTransform>::New();
  indexToWorldTransform->SetMatrix(indexToWorldMatrix);

  double planeOrigin[3]{}, planeRight[3]{}, planeBottom[3]{};
  auto fillArray3 = [](double filledArray[3], const std::initializer_list<double> fillList)
  {
    filledArray[0] = *fillList.begin();
    filledArray[1] = *(fillList.begin() + 1);
    filledArray[2] = *(fillList.begin() + 2);
  };
  int width = 0;
  int height = 0;
  switch (planeType)
  {
    case StandardPlane::Axial:
      fillArray3(planeRight, { 1, 0, 0 });
      fillArray3(planeBottom, { 0, 1, 0 });
      fillArray3(planeOrigin, { 0, 0, 0.5 });
      width = extents[0];
      height = extents[1];
      break;
    case StandardPlane::Sagittal:
      fillArray3(planeRight, { 0, 1, 0 });
      fillArray3(planeBottom, { 0, 0, 1 });
      fillArray3(planeOrigin, { 0.5, 0, 0 });
      width = extents[1];
      height = extents[2];
      break;
    case StandardPlane::Coronal:
      fillArray3(planeRight, { 1, 0, 0 });
      fillArray3(planeBottom, { 0, 0, 1 });
      fillArray3(planeOrigin, { 0, 0.5, 0 });
      width = extents[0];
      height = extents[2];
      break;
    default:
      SPDLOG_ERROR("plane type error");
      break;
  }

  double bounds[6] = { 0, static_cast<double>(width - 1), 0, static_cast<double>(height - 1), 0,
    1 };
  Superclass::setBounds(bounds);

  auto setMatrixColumn = [](vtkMatrix4x4* matrix, double* direction, int column)
  {
    matrix->SetElement(0, column, direction[0]);
    matrix->SetElement(1, column, direction[1]);
    matrix->SetElement(2, column, direction[2]);
  };
  double rightDirection[3]{};
  double bottomDirection[3]{};
  double planeOriginWorld[3]{};
  std::copy_n(indexToWorldTransform->TransformDoubleVector(planeRight), 3, rightDirection);
  std::copy_n(indexToWorldTransform->TransformDoubleVector(planeBottom), 3, bottomDirection);
  std::copy_n(indexToWorldTransform->TransformDoublePoint(planeOrigin), 3, planeOriginWorld);

  double normal[3];
  vtkMath::Cross(rightDirection, bottomDirection, normal);

  setMatrixColumn(indexToWorldMatrix, rightDirection, 0);
  setMatrixColumn(indexToWorldMatrix, bottomDirection, 1);
  setMatrixColumn(indexToWorldMatrix, normal, 2);
  setMatrixColumn(indexToWorldMatrix, planeOriginWorld, 3);

  indexToWorldTransform->SetMatrix(indexToWorldMatrix);
  Superclass::setIndexToWorldTransform(indexToWorldTransform);
}

void PlaneGeometry::normalizeMatrixColumn(vtkMatrix3x3* normalizedMatrix)
{
  for (size_t i = 0; i < 3; i++)
  {
    double column[3] = { normalizedMatrix->GetElement(0, i), normalizedMatrix->GetElement(1, i),
      normalizedMatrix->GetElement(2, i) };
    vtkMath::Normalize(column);
    normalizedMatrix->SetElement(0, i, column[0]);
    normalizedMatrix->SetElement(1, i, column[1]);
    normalizedMatrix->SetElement(2, i, column[2]);
  }
}

vtkVector3d PlaneGeometry::getNormal() const
{
  auto normal = Superclass::getAxisVector(Axis::Z);
  normal.Normalize();

  return normal;
}

double PlaneGeometry::SignedDistance(const double point[3]) const
{
  double origin[3];
  Superclass::getOrigin(origin);
  auto vector = vtkVector3d(point[0] - origin[0], point[1] - origin[1], point[2] - origin[2]);
  auto normal = getNormal();
  double distance = vector.Dot(normal);

  return distance;
}

bool PlaneGeometry::isAbove(const double point[3]) const
{
  return SignedDistance(point) > 0;
}

bool PlaneGeometry::IntersectionPoint(
  const double lineOrigin[3], const vtkVector3d& lineDirection, double intersectPoint[3]) const
{
  double t;
  if (!IntersectionParametricPoint(lineOrigin, lineDirection, t))
  {
    return false;
  }

  intersectPoint[0] = lineOrigin[0] + t * lineDirection.GetX();
  intersectPoint[1] = lineOrigin[1] + t * lineDirection.GetY();
  intersectPoint[2] = lineOrigin[2] + t * lineDirection.GetZ();

  return true;
}

bool PlaneGeometry::IntersectionParametricPoint(
  const double lineOrigin[3], const vtkVector3d& lineDirection, double& t) const
{
  auto planeNormal = getNormal();
  double planeOrigin[3];
  getOrigin(planeOrigin);
  const auto denom = planeNormal.Dot(lineDirection);

  const auto numer =
    planeNormal.Dot(vtkVector3d(planeOrigin)) - planeNormal.Dot(vtkVector3d(lineOrigin));
  if (std::abs(denom) < 1e-12)
  {
    return false;
  }

  t = numer / denom;

  return true;
}

bool PlaneGeometry::IntersectionLine(
  const PlaneGeometry* planeGeometry, double lineOrigin[3], vtkVector3d& lineDirection) const
{
  auto normal = getNormal();
  auto planeNormal = planeGeometry->getNormal();

  lineDirection = normal.Cross(planeNormal);

  if (lineDirection.SquaredNorm() < 1e-12)
  {
    return false;
  }

  auto N1dN2 = normal.Dot(planeNormal);
  auto determinant = 1.0 - N1dN2 * N1dN2;

  double origin[3], planeOrigin[3];
  getOrigin(origin);
  planeGeometry->getOrigin(planeOrigin);

  auto d1 = normal.Dot(vtkVector3d(origin));
  auto d2 = planeNormal.Dot(vtkVector3d(planeOrigin));

  auto c1 = (d1 - d2 * N1dN2) / determinant;
  auto c2 = (d2 - d1 * N1dN2) / determinant;

  lineOrigin[0] = normal.GetX() * c1 + planeNormal.GetX() * c2;
  lineOrigin[1] = normal.GetY() * c1 + planeNormal.GetY() * c2;
  lineOrigin[2] = normal.GetZ() * c1 + planeNormal.GetZ() * c2;

  return true;
}

double PlaneGeometry::angle(const PlaneGeometry* planeGeometry) const
{
  auto normal = getNormal();
  auto planeNormal = planeGeometry->getNormal();

  double rad = vtkMath::AngleBetweenVectors(normal.GetData(), planeNormal.GetData());

  return rad;
}

double PlaneGeometry::angle(const vtkVector3d& lineDirection) const
{
  auto normal = getNormal();
  return vtkMath::Pi() * 0.5 -
    vtkMath::AngleBetweenVectors(lineDirection.GetData(), normal.GetData());
}

bool PlaneGeometry::isParallel(const PlaneGeometry* planeGeometry)
{
  auto normal = getNormal();
  auto planeNormal = planeGeometry->getNormal();
  double cross[3];
  vtkMath::Cross(normal.GetData(), planeNormal.GetData(), cross);

  return vtkMath::Norm(cross) < 1e-6;
}

bool PlaneGeometry::isOnPlane(const double lineOrigin[3], const vtkVector3d& lineDirection) const
{
  auto normal = getNormal();
  auto lineOriginOnPlane = isOnPlane(lineOrigin);
  auto normalParallel = (std::abs(normal.Dot(lineDirection)) < 1e-6);
  return lineOriginOnPlane && normalParallel;
}

bool PlaneGeometry::isOnPlane(const double point[3]) const
{
  auto normal = getNormal();
  double origin[3];
  getOrigin(origin);
  auto d = vtkMath::Dot(normal.GetData(), origin);
  auto d1 = vtkMath::Dot(normal.GetData(), point);
  return (std::abs(d - d1) < 1e-6);
}

void PlaneGeometry::projectToPlane(const double point[3], double projectedPoint[3])
{
  double origin[3];
  getOrigin(origin);
  // ensures the normal is normalized
  auto normal = getNormal();
  double n[3] = { normal.GetX(), normal.GetY(), normal.GetZ() };

  double projectVector[3];
  vtkMath::Subtract(point, origin, projectVector);
  auto length = vtkMath::Dot(projectVector, n);

  vtkMath::MultiplyScalar(n, -length);
  vtkMath::Add(point, n, projectedPoint);
}

bool PlaneGeometry::map(const double point[3], double mappedPoint[2])
{
  return false;
}

PlaneGeometry::PlaneGeometry() {}

PlaneGeometry::~PlaneGeometry() {}

std::array<int, 3> PlaneGeometry::calculateDominantAxis(vtkMatrix3x3* worldToIndexMatrix)
{
  std::array<int, 3> axes;
  bool domain_axis_error = false;

  /**
   * 注意这里的计算，因为我们使用的是WorldToIndex矩阵，所以每一列是世界坐标系的轴在索引坐标系下的定义，
   * 但是，因为矩阵是正交矩阵，所以worldToIndex =
   * IndexToWorld.Transpose，所以worldToIndex的每一行代表的是索引坐标系在世界坐标系下的向量
   */
  for (size_t i = 0; i < 3; i++)
  {
    double axisArray[3] = { std::abs(worldToIndexMatrix->GetElement(0, i)),
      std::abs(worldToIndexMatrix->GetElement(1, i)),
      std::abs(worldToIndexMatrix->GetElement(2, i)) };
    auto it = std::max_element(axisArray, axisArray + 3);
    int domainAxis = std::distance(axisArray, it);

    // all axis must perpendicular, otherwise, there will use the default axis
    for (size_t j = 0; j < i; j++)
    {
      if (axes[i] == domainAxis)
      {
        domain_axis_error = true;
        break;
      }
    }

    if (domain_axis_error)
    {
      break;
    }

    axes[i] = domainAxis;
  }

  if (domain_axis_error)
  {
    SPDLOG_WARN("domain axis find error, use default axis");
    for (size_t i = 0; i < 3; i++)
    {
      axes[i] = i;
    }
  }

  return axes;
}
