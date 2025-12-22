#ifndef PLANE_GEOMETRY_H
#define PLANE_GEOMETRY_H
#include "BaseGeometry.h"
#include "MultiPlanarReconstructExport.h"
#include "PlaneType.h"
#include <array>
#include <memory>
#include <vtkLine.h>

class vtkTransform;
class vtkImageData;
class vtkMatrix3x3;
class MULTIPLANARRECONSTRUCT_API PlaneGeometry : public BaseGeometry
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
  static void normalizeMatrixColumn(vtkMatrix3x3* normalizedMatrix);
  vtkVector3d getNormal() const;
  double SignedDistance(const double point[3]) const;
  [[nodiscard]] bool isAbove(const double point[3]) const;
  [[nodiscard]] bool IntersectionPoint(
    const double lineOrigin[3], const vtkVector3d& lineDirection, double intersectPoint[3]) const;
  [[nodiscard]] bool IntersectionParametricPoint(
    const double lineOrigin[3], const vtkVector3d& lineDirection, double& t) const;
  [[nodiscard]] bool IntersectionLine(
    const PlaneGeometry* planeGeometry, double lineOrigin[3], vtkVector3d& lineDirection) const;
  [[nodiscard]] double angle(const PlaneGeometry* planeGeometry) const;
  [[nodiscard]] double angle(const vtkVector3d& lineDirection) const;
  [[nodiscard]] bool isParallel(const PlaneGeometry* planeGeometry);
  [[nodiscard]] bool isOnPlane(const double lineOrigin[3], const vtkVector3d& lineDirection) const;
  [[nodiscard]] bool isOnPlane(const double point[3]) const;
  [[nodiscard]] void projectToPlane(const double point[3], double projectedPoint[3]);
  [[nodiscard]] bool map(const double point[3], double mappedPoint[2]);

  /**
   * @brief calculate the domain axis to determain the spacing and number of slice. It will return
   * the image index axis' closest world axis
   */
  static std::array<int, 3> calculateDominantAxis(vtkMatrix3x3* worldToIndexMatrix);

protected:
  PlaneGeometry();
  ~PlaneGeometry() override;
};

#endif // PLANE_GEOMETRY_H
