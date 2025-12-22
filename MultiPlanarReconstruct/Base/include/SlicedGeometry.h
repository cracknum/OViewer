#ifndef SLICED_GEOMETRY_H
#define SLICED_GEOMETRY_H
#include "BaseExport.h"
#include "BaseGeometry.h"
#include "PlaneType.h"
#include "SlicedGeometry.h"
#include <memory>

class vtkImageData;
class PlaneGeometry;
class BASE_API SlicedGeometry final : public BaseGeometry
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