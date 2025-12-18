#include "PlaneGeometry.h"
#include <vtkMath.h>

struct PlaneGeometry::Private
{
};

void PlaneGeometry::initializePlane(double bounds[4], vtkTransform* indexToWorldTransform)
{
  double bounds3D[6] = { bounds[0], bounds[1], bounds[2], bounds[4], 0, 0 };
  Superclass::setBounds(bounds);
  Superclass::setIndexToWorldTransform(indexToWorldTransform);
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
  double n[3] = {normal.GetX(), normal.GetY(), normal.GetZ()};

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

bool PlaneGeometry::map(const double point[2], double mappedPoint[3])
{
  return false;
}

PlaneGeometry::PlaneGeometry()
{
  mPrivate = std::make_unique<Private>();
}

PlaneGeometry::~PlaneGeometry() {}
