#ifndef PLANE_LOCAL_BOUNDS_FILTER_H
#define PLANE_LOCAL_BOUNDS_FILTER_H
#include <vtkPolyDataAlgorithm.h>
#include <vtkSmartPointer.h>
#include <vtkTransform.h>
#include "FiltersExport.h"

class vtkPoints;
class vtkVector;

class FILTERS_API PlaneLocalBoundsFilter : public vtkPolyDataAlgorithm
{
public:
  static PlaneLocalBoundsFilter* New();
  vtkTypeMacro(PlaneLocalBoundsFilter, vtkPolyDataAlgorithm);
  void PrintSelf(ostream& os, vtkIndent indent) override;

  vtkSetSmartPointerMacro(PlaneLocalToWorldTransform, vtkTransform);

protected:
  PlaneLocalBoundsFilter();
  ~PlaneLocalBoundsFilter() override;

  int RequestData(vtkInformation* request, vtkInformationVector** inputVector,
    vtkInformationVector* outputVector) override;

  int FillOutputPortInformation(int port, vtkInformation* info) override;
  int FillInputPortInformation(int port, vtkInformation* info) override;

  static bool LineIntersectZero(vtkPoints *points, int p1, int p2, double *bounds);

private:
  vtkSmartPointer<vtkTransform> PlaneLocalToWorldTransform;
};
#endif // PLANE_LOCAL_BOUNDS_FILTER_H