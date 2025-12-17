#include "PlaneGeometry.h"

struct PlaneGeometry::Private
{
};

void PlaneGeometry::initializePlane(double bounds[4], vtkTransform* indexToWorldTransform)
{
	double bounds3D[6] = {bounds[0], bounds[1], bounds[2], bounds[4], 0, 0};
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

bool PlaneGeometry::IntersectionPoint(const double lineOrigin[3], const vtkVector3d& lineDirection, double intersectPoint[3]) const
{
	double t;
	if(!IntersectionParametricPoint(lineOrigin, lineDirection, t))
	{
		return false;
	}

	intersectPoint[0] = lineOrigin[0] + t * lineDirection.GetX();
	intersectPoint[1] = lineOrigin[1] + t * lineDirection.GetY();
	intersectPoint[2] = lineOrigin[2] + t * lineDirection.GetZ();
	
    return true;
}

bool PlaneGeometry::IntersectionParametricPoint(const double lineOrigin[3], const vtkVector3d& lineDirection, double &t) const
{
   	auto planeNormal = getNormal();
	double planeOrigin[3];
	getOrigin(planeOrigin);
	const auto denom = planeNormal.Dot(lineDirection);

	const auto numer = planeNormal.Dot(vtkVector3d(planeOrigin)) - planeNormal.Dot(vtkVector3d(lineOrigin));
	if (std::abs(denom) < 1e-12)
	{
		return false;
	}

	t = numer / denom;

	return true;
}

bool PlaneGeometry::IntersectionLine(const PlaneGeometry* planeGeometry, double startPoint[3], double endPoint[3]) const
{
	
    return false;
}

double PlaneGeometry::angle(const PlaneGeometry* planeGeometry) const
{
    return 0.0;
}

double PlaneGeometry::angle(const double startPoint[3], const double endPoint[3]) const
{
    return 0.0;
}

bool PlaneGeometry::isParallel(const PlaneGeometry* planeGeometry)
{
    return false;
}

bool PlaneGeometry::isOnPlane(const double startPoint[3], const double endPoint[3]) const
{
    return false;
}

bool PlaneGeometry::isOnPlane(const double point[3]) const
{
    return false;
}

void PlaneGeometry::projectToPlane(const double point[3], double projectedPoint[3]) {}

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
