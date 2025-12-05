#include "ImageResliceFilter.h"
#include "ImageResliceFilterCuda.cuh"
#include "Plane.h"
#include "Volume.h"
#include <glm/glm.hpp>
#include <vtkDemandDrivenPipeline.h>
#include <vtkImageData.h>
#include <vtkInformation.h>
#include <vtkInformationIntegerKey.h>
#include <vtkInformationKey.h>
#include <vtkInformationObjectBaseKey.h>
#include <vtkInformationVector.h>
#include <vtkMatrix3x3.h>
#include <vtkMatrix4x4.h>
#include <vtkPointSet.h>
#include <vtkTransform.h>
#include <spdlog/spdlog.h>

vtkStandardNewMacro(ImageResliceFilter);
vtkInformationKeyMacro(ImageResliceFilter, TEXTURE_SIZE, Integer);
vtkInformationKeyMacro(ImageResliceFilter, PLANE_BOUNDS_POINTS, ObjectBase);

namespace
{
glm::mat4 convertToMat4(const vtkMatrix4x4* matrix)
{
  glm::mat4 mat;
  for (int col = 0; col < 4; col++)
  {
    for (int row = 0; row < 4; row++)
    {
      mat[col][row] = static_cast<float>(matrix->GetElement(row, col));
    }
  }
  return mat;
}
}

//----------------------------------------------------------------------
void ImageResliceFilter::PrintSelf(ostream& os, vtkIndent indent)
{
  Superclass::PrintSelf(os, indent);
}

//----------------------------------------------------------------------
ImageResliceFilter::ImageResliceFilter()
  : ImageResliceFilterAlgo(nullptr)
{
  // input 0: volume data : vtkImageData
  this->SetNumberOfInputPorts(1);
  // output 0: vtkPolyData with plane's local bounds array 2D
  this->SetNumberOfOutputPorts(1);
}

//----------------------------------------------------------------------
ImageResliceFilter::~ImageResliceFilter() {}

//----------------------------------------------------------------------
int ImageResliceFilter::RequestData(vtkInformation* vtkNotUsed(request),
  vtkInformationVector** inputVector, vtkInformationVector* outputVector)
{
  auto inputInform = inputVector[0]->GetInformationObject(0);
  auto outputInform = outputVector->GetInformationObject(0);
  auto imageData = vtkImageData::SafeDownCast(inputInform->Get(vtkDataObject::DATA_OBJECT()));
  auto outputImage = vtkImageData::SafeDownCast(outputInform->Get(vtkDataObject::DATA_OBJECT()));
  if (!imageData || !PlaneLocalToWorldTransform)
  {
    vtkErrorMacro("image data or plane local to world transform is not set");
    return 0;
  }

  int textureSize = inputInform->Get(TEXTURE_SIZE());
  auto bounds = vtkPoints::SafeDownCast(outputInform->Get(PLANE_BOUNDS_POINTS()));
  if (bounds->GetNumberOfPoints() < 2)
  {
    vtkErrorMacro("plane and image data is not intersect");
    return 0;
  }

  if (!ImageResliceFilterAlgo)
  {
    ImageResliceFilterAlgo = std::make_unique<ImageResliceFilterCuda>();
  }

  double origin[3], spacing[3];
  int dimensions[3];
  imageData->GetOrigin(origin);
  imageData->GetSpacing(spacing);
  imageData->GetDimensions(dimensions);
  SPDLOG_INFO("read imageData");

  double planeIndexOrigin[3] = { 0, 0, 0 };
  double* planeWorldOrigin = PlaneLocalToWorldTransform->TransformDoublePoint(planeIndexOrigin);
  std::shared_ptr<Plane> plane = std::make_shared<Plane>(
    glm::vec3(planeWorldOrigin[0], planeWorldOrigin[2], planeWorldOrigin[1]),
    glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0),
    convertToMat4(PlaneLocalToWorldTransform->GetMatrix()));

  double minBound[3], maxBound[3];
  bounds->GetPoint(0, minBound);
  bounds->GetPoint(1, maxBound);
  plane->m_Width = maxBound[0] - minBound[0] + 1;
  plane->m_Height = maxBound[1] - minBound[1] + 1;
  SPDLOG_INFO("plane width: {}, height: {}", plane->m_Width, plane->m_Height);

  std::shared_ptr<Volume> volume = std::make_shared<Volume>(
    glm::vec3(origin[0], origin[1], origin[2]), glm::vec3(spacing[0], spacing[1], spacing[2]),
    glm::ivec3(dimensions[0], dimensions[1], dimensions[2]), DataType::FLOAT,
    imageData->GetScalarPointer(), convertToMat4(imageData->GetIndexToPhysicalMatrix()));

  ImageResliceFilterAlgo->setPlane(plane);
  ImageResliceFilterAlgo->setVolume(volume);
  ImageResliceFilterAlgo->doFilter();

  SPDLOG_INFO("finished reslice");

  outputImage->SetDimensions(plane->m_Width, plane->m_Height, 1);

  float scale[3];
  PlaneLocalToWorldTransform->GetScale(scale);
  outputImage->SetSpacing(scale[0], scale[1], scale[2]);
  outputImage->SetOrigin(planeWorldOrigin);
  outputImage->AllocateScalars(imageData->GetScalarType(), 1);

  const void* pixels = ImageResliceFilterAlgo->getPixels();
  memcpy(outputImage->GetScalarPointer(), pixels,
    plane->m_Width * plane->m_Height * getDataTypeSize(DataType::FLOAT));
  
  SPDLOG_INFO("finished");
  return 1;
}

//----------------------------------------------------------------------
int ImageResliceFilter::RequestInformation(vtkInformation* vtkNotUsed(request),
  vtkInformationVector** inputVector, vtkInformationVector* outputVector)
{
  auto inputInform = inputVector[0]->GetInformationObject(0);
  auto outputInform = outputVector->GetInformationObject(0);
  auto imageData = vtkImageData::SafeDownCast(inputInform->Get(vtkDataObject::DATA_OBJECT()));
  vtkSmartPointer<vtkPoints> boundPoints = vtkSmartPointer<vtkPoints>::New();

  // calculate max texture size
  int dimensions[3];
  imageData->GetDimensions(dimensions);
  int maxDiagonalLength2 = std::max({ dimensions[0] * dimensions[0], dimensions[1] * dimensions[1],
    dimensions[2] * dimensions[2] });
  int textureMaxSize = std::sqrt(maxDiagonalLength2);

  outputInform->Set(TEXTURE_SIZE(), textureMaxSize);
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
  // index 1 and 2: bounds
  boundPoints->InsertNextPoint(bounds[0], bounds[2], bounds[4]);
  boundPoints->InsertNextPoint(bounds[1], bounds[3], bounds[5]);
  outputInform->Set(PLANE_BOUNDS_POINTS(), boundPoints);

  return 1;
}

//----------------------------------------------------------------------
bool ImageResliceFilter::LineIntersectZero(vtkPoints* points, int p1, int p2, double* bounds)

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
