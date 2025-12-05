#ifndef PLANE_LOCAL_BOUNDS_FILTER_H
#define PLANE_LOCAL_BOUNDS_FILTER_H
#include <vtkImageAlgorithm.h>
#include <vtkSmartPointer.h>
#include <vtkTransform.h>
#include "FiltersExport.h"

class vtkPoints;
class vtkVector;
class vtkInformationIntegerKey;
class vtkInformationObjectBaseKey;
class ImageResliceFilterCuda;

class FILTERS_API ImageResliceFilter : public vtkImageAlgorithm
{
public:
  static ImageResliceFilter* New();
  vtkTypeMacro(ImageResliceFilter, vtkImageAlgorithm);
  void PrintSelf(ostream& os, vtkIndent indent) override;

  vtkSetSmartPointerMacro(PlaneLocalToWorldTransform, vtkTransform);

protected:
  ImageResliceFilter();
  ~ImageResliceFilter() override;

  static vtkInformationIntegerKey* TEXTURE_SIZE();
  static vtkInformationObjectBaseKey* PLANE_BOUNDS_POINTS();

  int RequestData(vtkInformation* request, vtkInformationVector** inputVector,
      vtkInformationVector* outputVector) override;
  int RequestInformation(vtkInformation* request, vtkInformationVector** inputVector,
    vtkInformationVector* outputVector) override;

  static bool LineIntersectZero(vtkPoints *points, int p1, int p2, double *bounds);

private:
  vtkSmartPointer<vtkTransform> PlaneLocalToWorldTransform;
  std::unique_ptr<ImageResliceFilterCuda> ImageResliceFilterAlgo;
};
#endif // PLANE_LOCAL_BOUNDS_FILTER_H