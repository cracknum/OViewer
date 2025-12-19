#ifndef SLICED_GEOMETRY_H
#define SLICED_GEOMETRY_H
#include "SlicedGeometry.h"
#include "BaseGeometry.h"
#include <memory>

class vtkImageData;
class PlaneGeometry;
class SlicedGeometry : public BaseGeometry
{
public:
  static SlicedGeometry* New();
  vtkTypeMacro(SlicedGeometry, BaseGeometry);
  
  void initialize(vtkImageData* imageData);
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
  void initializePlanes(const double planeNormal[3]);
  private:
  struct Private;
  std::unique_ptr<Private> mPrivate;
};

#endif // SLICED_GEOMETRY_H