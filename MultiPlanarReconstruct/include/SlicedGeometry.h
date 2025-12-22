#ifndef SLICED_GEOMETRY_H
#define SLICED_GEOMETRY_H
#include "BaseGeometry.h"
#include "SlicedGeometry.h"
#include "PlaneType.h"
#include <memory>
#include "MultiPlanarReconstructExport.h"

class vtkImageData;
class PlaneGeometry;
class MULTIPLANARRECONSTRUCT_API SlicedGeometry : public BaseGeometry
{
public:
  static SlicedGeometry* New();
  vtkTypeMacro(SlicedGeometry, BaseGeometry);

  void initialize(vtkImageData* imageData, StandardPlane planeType);
  void reinitialize();

  PlaneGeometry* getPlaneGeometry(int sliceNumber) const;
  unsigned int getNumberOfSlices() const;
  bool isValidSlice(int sliceNumber) const;

protected:
  SlicedGeometry();
  ~SlicedGeometry() override;

  /**
   * @brief calculate the target direction spacing
   */
  static double calculateSpacing(const double spacing[3], const vtkVector3d& direction);
  void reinitializePlanes();
  vtkSmartPointer<PlaneGeometry> initializeStandardPlanes();

private:
  struct Private;
  std::unique_ptr<Private> mPrivate;
};

#endif // SLICED_GEOMETRY_H