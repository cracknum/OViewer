#ifndef MPR_BASE_GEOMETRY_H
#define MPR_BASE_GEOMETRY_H
#include "MultiPlanarReconstructExport.h"
#include <memory>
#include <vtkObject.h>
#include <vtkVector.h>


class vtkTransform;
class vtkMatrix3x3;
/**
 * @brief BaseGeometry is the base class to represent a object state
 */
class MULTIPLANARRECONSTRUCT_API BaseGeometry : public vtkObject
{
public:
  static BaseGeometry* New();

  enum Axis
  {
    X,
    Y,
    Z
  };

  void getOrigin(double origin[3]) const;
  void setOrigin(double origin[3]);
  void getSpacing(double spacing[3]) const;
  double getSpacing(Axis axis) const;
  void setSpacing(double spacing[3]);

  vtkTransform* getIndexToWorldTransform();
  vtkTransform* getWorldToIndexTransform();
  void setIndexToWorldTransform(vtkTransform* transform);

  void getBounds(double bounds[6]) const;
  /**
   * @brief set bounds in index coordiantes, not world coordinates
   */
  void setBounds(double bounds[6]);
  [[nodiscard]] double getExtent(Axis axis) const;
  [[nodiscard]] double getExtentInMM(Axis axis) const;
  /**
   * @brief get geometry center in world coordinates(mm)
   */
  void getCenter(double center[3]) const;
  /**
   * @brief get axis direction vector
   */
  vtkVector3d getAxisVector(Axis axis) const;
  /**
   * @brief whether the geometry is image geometry
   */
  void setImageGeometry(bool imageGeometry);
  /**
   * @brief frame of reference is important for data alignment
   */
  void setFrameOfReferenceId(unsigned int frameOfReference);
  unsigned int getFrameOfReferenceId() const;

  vtkMatrix3x3* getLinearTransformMatrix() const;

protected:
  BaseGeometry();
  ~BaseGeometry() override;

private:
  struct Private;
  std::unique_ptr<Private> mPrivate;
};
#endif // MPR_BASE_GEOMETRY_H