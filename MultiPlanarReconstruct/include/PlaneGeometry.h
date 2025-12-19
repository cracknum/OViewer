#ifndef PLANE_GEOMETRY_H
#define PLANE_GEOMETRY_H
#include "BaseGeometry.h"
#include <memory>
#include <vtkLine.h>
#include <array>

class vtkTransform;
class vtkImageData;
class vtkMatrix3x3;
class PlaneGeometry : public BaseGeometry
{
	public:
  enum class StandardPlane
  {
	Axial,
	Sagittal,
	Coronal
  };

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
  /**
   * @brief initialize having arbitrary normal plane
   */
  void initializePlane(vtkImageData* imageData, const double planeNormal[3]);
  /**
   * @brief initialize standard plane, which means it would be one of axial, sagittal and coronal
   */
  void initializeStandardPlane(vtkImageData* imageData, StandardPlane planeType);
  /**
   * @brief get the plane normal, the method ensures the result is normalized
   */
  vtkVector3d getNormal() const;
  double SignedDistance(const double point[3]) const;
  [[nodiscard]] bool isAbove(const double point[3]) const;
  [[nodiscard]] bool IntersectionPoint(const double lineOrigin[3], const vtkVector3d& lineDirection, double intersectPoint[3]) const;
  [[nodiscard]] bool IntersectionParametricPoint(const double lineOrigin[3], const vtkVector3d& lineDirection, double &t) const;
  [[nodiscard]] bool IntersectionLine(const PlaneGeometry* planeGeometry, double lineOrigin[3], vtkVector3d& lineDirection) const;
  [[nodiscard]] double angle(const PlaneGeometry* planeGeometry) const;
  [[nodiscard]] double angle(const vtkVector3d& lineDirection) const;
  [[nodiscard]] bool isParallel(const PlaneGeometry* planeGeometry);
  [[nodiscard]] bool isOnPlane(const double lineOrigin[3], const vtkVector3d& lineDirection) const;
  [[nodiscard]] bool isOnPlane(const double point[3]) const;
  [[nodiscard]] void projectToPlane(const double point[3], double projectedPoint[3]);
  [[nodiscard]] bool map(const double point[3], double mappedPoint[2]);
  [[nodiscard]] bool map(const double point[2], double mappedPoint[3]);
protected:
  PlaneGeometry();
  ~PlaneGeometry() override;

  /**
   * @brief calculate the domain axis to determain the spacing and number of slice. It will return the image index axis' closest world axis
   */
  std::array<int, 3> calculateDominantAxis(vtkMatrix3x3* worldToIndexMatrix);

private:
  struct Private;
  std::unique_ptr<Private> mPrivate;
};

#endif // PLANE_GEOMETRY_H