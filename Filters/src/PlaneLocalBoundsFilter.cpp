#include "PlaneLocalBoundsFilter.h"
#include <vtkDemandDrivenPipeline.h>
#include <vtkImageData.h>
#include <vtkInformation.h>
#include <vtkInformationVector.h>
#include <vtkMatrix3x3.h>
#include <vtkMatrix4x4.h>
#include <vtkPointSet.h>
#include <vtkTransform.h>

vtkStandardNewMacro(PlaneLocalBoundsFilter);

//----------------------------------------------------------------------
void PlaneLocalBoundsFilter::PrintSelf(ostream& os, vtkIndent indent)
{
  Superclass::PrintSelf(os, indent);
}

//----------------------------------------------------------------------
PlaneLocalBoundsFilter::PlaneLocalBoundsFilter()
{
  // input 0: volume data : vtkImageData
  this->SetNumberOfInputPorts(1);
  // output 0: vtkPolyData with plane's local bounds array 2D
  this->SetNumberOfOutputPorts(1);
}

//----------------------------------------------------------------------
PlaneLocalBoundsFilter::~PlaneLocalBoundsFilter() {}

//----------------------------------------------------------------------
int PlaneLocalBoundsFilter::RequestData(vtkInformation* vtkNotUsed(request),
  vtkInformationVector** inputVector, vtkInformationVector* outputVector)
{
  auto inputInform = inputVector[0]->GetInformationObject(0);
  auto outputInform = outputVector->GetInformationObject(0);
  auto imageData = vtkImageData::SafeDownCast(inputInform->Get(vtkDataObject::DATA_OBJECT()));
  auto boundsSet = vtkPolyData::SafeDownCast(outputInform->Get(vtkDataObject::DATA_OBJECT()));

  if (!imageData || !boundsSet)
  {
    return 0;
  }

  vtkSmartPointer<vtkPoints> boundPoints = vtkSmartPointer<vtkPoints>::New();
  boundsSet->SetPoints(boundPoints);

  // get image data bounds in world and PhysicalToIndexMatrix
  double imageDataBounds[6]{};
  double bounds[6]{ std::numeric_limits<double>::max(), std::numeric_limits<double>::min(),
    std::numeric_limits<double>::max(), std::numeric_limits<double>::min(), 0, 0 };
  imageData->GetBounds(imageDataBounds);
  double bbMin[3] = { imageDataBounds[0], imageDataBounds[2], imageDataBounds[4] };
  double bbMax[3] = { imageDataBounds[1], imageDataBounds[3], imageDataBounds[5] };

  vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
  vtkSmartPointer<vtkPoints> newPoints = vtkSmartPointer<vtkPoints>::New();
  points->InsertPoint(0, bbMin[0], bbMin[1], bbMin[2]);
  points->InsertPoint(1, bbMin[0], bbMin[1], bbMax[2]);
  points->InsertPoint(2, bbMin[0], bbMax[1], bbMax[2]);
  points->InsertPoint(3, bbMin[0], bbMax[1], bbMin[2]);
  points->InsertPoint(4, bbMax[0], bbMin[1], bbMin[2]);
  points->InsertPoint(5, bbMax[0], bbMin[1], bbMax[2]);
  points->InsertPoint(6, bbMax[0], bbMax[1], bbMax[2]);
  points->InsertPoint(7, bbMax[0], bbMax[1], bbMin[2]);

  // concate transform for imageData bounds from imageData world coordinates to plane local
  // coordinates
  vtkSmartPointer<vtkTransform> transform = vtkSmartPointer<vtkTransform>::New();
  vtkSmartPointer<vtkMatrix4x4> planeWorldToLocalMatrix = vtkSmartPointer<vtkMatrix4x4>::New();
  if (!PlaneLocalToWorldTransform || !PlaneLocalToWorldTransform->GetMatrix())
  {
    std::cout << "transform or matrix get error: PlaneLocalToWorldTransform is nullptr: "
              << (PlaneLocalToWorldTransform == nullptr)
              << " matrix is nullptr: " << (PlaneLocalToWorldTransform->GetMatrix() == nullptr)
              << std::endl;
    return 0;
  }

  planeWorldToLocalMatrix->DeepCopy(PlaneLocalToWorldTransform->GetMatrix());
  planeWorldToLocalMatrix->Invert();
  transform->Concatenate(planeWorldToLocalMatrix);
  transform->TransformPoints(points, newPoints);

  LineIntersectZero(newPoints, 0, 1, bounds);
  LineIntersectZero(newPoints, 1, 2, bounds);
  LineIntersectZero(newPoints, 2, 3, bounds);
  LineIntersectZero(newPoints, 3, 0, bounds);
  LineIntersectZero(newPoints, 0, 4, bounds);
  LineIntersectZero(newPoints, 1, 5, bounds);
  LineIntersectZero(newPoints, 2, 6, bounds);
  LineIntersectZero(newPoints, 3, 7, bounds);
  LineIntersectZero(newPoints, 4, 5, bounds);
  LineIntersectZero(newPoints, 5, 6, bounds);
  LineIntersectZero(newPoints, 6, 7, bounds);
  LineIntersectZero(newPoints, 7, 4, bounds);

  if ((bounds[0] > 9999999.0) || (bounds[2] > 9999999.0) || (bounds[1] < -9999999.0) ||
    (bounds[3] < -9999999.0))
  {
    return 1;
  }
  
  boundPoints->InsertNextPoint(bounds[0], bounds[2], bounds[4]);
  boundPoints->InsertNextPoint(bounds[1], bounds[3], bounds[5]);

  return 1;
}

//----------------------------------------------------------------------
int PlaneLocalBoundsFilter::FillInputPortInformation(int port, vtkInformation* info)
{
  int modified = 0;
  if (port == 0)
  {
    info->Set(vtkAlgorithm::INPUT_REQUIRED_DATA_TYPE(), "vtkImageData");
    modified = 1;
  }

  return modified;
}

bool PlaneLocalBoundsFilter::LineIntersectZero(vtkPoints* points, int p1, int p2, double* bounds)

{
  double point1[3];
  double point2[3];
  points->GetPoint(p1, point1);
  points->GetPoint(p2, point2);

  if ((point1[2] * point2[2] <= 0.0) && (point1[2] != point2[2]))
  {
    double x, y;
    x = (point1[0] * point2[2] - point1[2] * point2[0]) / (point2[2] - point1[2]);
    y = (point1[1] * point2[2] - point1[2] * point2[1]) / (point2[2] - point1[2]);

    if (x < bounds[0])
    {
      bounds[0] = x;
    }
    if (x > bounds[1])
    {
      bounds[1] = x;
    }
    if (y < bounds[2])
    {
      bounds[2] = y;
    }
    if (y > bounds[3])
    {
      bounds[3] = y;
    }
    bounds[4] = bounds[5] = 0.0;
    return true;
  }
  return false;
}

//----------------------------------------------------------------------
int PlaneLocalBoundsFilter::FillOutputPortInformation(int port, vtkInformation* info)
{
  int modified = 0;
  if (port == 0)
  {
    info->Set(vtkDataObject::DATA_TYPE_NAME(), "vtkPolyData");
    modified = 1;
  }

  return modified;
}