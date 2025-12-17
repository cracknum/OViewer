#ifndef PLANE_GEOMETRY_H
#define PLANE_GEOMETRY_H
#include "BaseGeometry.h"
#include <memory>
#include <vtkLine.h>

class vtkTransform;
class PlaneGeometry : public BaseGeometry
{
public:
  vtkTypeMacro(PlaneGeometry, BaseGeometry);
  static PlaneGeometry* New();

  /**
   * @brief initialize PlaneGeometry
   * @param bounds plane bounds in index coordinates
   * @param indexToWorldTransform transform index coordinates to world coordinates, plane is defined
   * in xy plane
   */
  void initializePlane(double bounds[4], vtkTransform* indexToWorldTransform);
  vtkVector3d getNormal() const;
  double SignedDistance(const double point[3]) const;
  [[nodiscard]] bool isAbove(const double point[3]) const;
  [[nodiscard]] bool IntersectionPoint(const double lineOrigin[3], const vtkVector3d& lineDirection, double intersectPoint[3]) const;
  [[nodiscard]] bool IntersectionParametricPoint(const double lineOrigin[3], const vtkVector3d& lineDirection, double &t) const;
  [[nodiscard]] bool IntersectionLine(const PlaneGeometry* planeGeometry, double startPoint[3], double endPoint[3]) const;
  [[nodiscard]] double angle(const PlaneGeometry* planeGeometry) const;
  [[nodiscard]] double angle(const double startPoint[3], const double endPoint[3]) const;
  [[nodiscard]] bool isParallel(const PlaneGeometry* planeGeometry);
  [[nodiscard]] bool isOnPlane(const double startPoint[3], const double endPoint[3]) const;
  [[nodiscard]] bool isOnPlane(const double point[3]) const;
  [[nodiscard]] void projectToPlane(const double point[3], double projectedPoint[3]);
  [[nodiscard]] bool map(const double point[3], double mappedPoint[2]);
  [[nodiscard]] bool map(const double point[2], double mappedPoint[3]);
protected:
  PlaneGeometry();
  ~PlaneGeometry() override;

private:
  struct Private;
  std::unique_ptr<Private> mPrivate;
};

#endif // PLANE_GEOMETRY_H