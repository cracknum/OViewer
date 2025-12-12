#include "ImageResliceFilter1.h"
#include "ImageResliceFilterCuda.cuh"
#include "Volume.h"
#include <Texture2DObject.h>
#include <spdlog/spdlog.h>
#include <vtkMatrix4x4.h>
#include <vtkPoints.h>
#include <vtkTransform.h>

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

bool sameMatrix(vtkMatrix4x4* matrix1, vtkMatrix4x4* matrix2)
{
  for (size_t i = 0; i < 4; i++)
  {
    for (size_t j = 0; j < 4; j++)
    {
      if (std::abs(matrix1->GetElement(i, j) - matrix2->GetElement(i, j)) > 0)
      {
        return false;
      }
    }
  }

  return true;
}
}

struct ImageResliceFilter1::Private
{
  std::shared_ptr<Texture2DObject> mTextureObject;
  std::unique_ptr<ImageResliceFilterCuda> mResliceFilterCuda;
  vtkSmartPointer<vtkTransform> mPlaneLocalToWorldTransform;
  vtkSmartPointer<vtkImageData> mImageData;

  bool mTrackingStatus;
  Private()
    : mTrackingStatus(false)
  {
    mResliceFilterCuda = std::make_unique<ImageResliceFilterCuda>();
  }
};

ImageResliceFilter1::ImageResliceFilter1()
{
  mPrivate = std::make_unique<Private>();
}

ImageResliceFilter1::~ImageResliceFilter1() = default;

void ImageResliceFilter1::setTexture(std::shared_ptr<Texture2DObject> textureObject)
{
  mPrivate->mTextureObject = std::move(textureObject);
}

bool ImageResliceFilter1::doFilter(
  vtkSmartPointer<vtkImageData> imageData, vtkSmartPointer<vtkTransform> planeLocalToWorldTransform)
{
  if (imageData == mPrivate->mImageData &&
    sameMatrix(
      planeLocalToWorldTransform->GetMatrix(), mPrivate->mPlaneLocalToWorldTransform->GetMatrix()))
  {
    return mPrivate->mTrackingStatus;
  }

  if (imageData != mPrivate->mImageData)
  {
    mPrivate->mImageData = std::move(imageData);
    // auto maxTextureSize = calculateMaxTextureSize(imageData);
  }
  if (planeLocalToWorldTransform != mPrivate->mPlaneLocalToWorldTransform)
  {
    mPrivate->mPlaneLocalToWorldTransform = std::move(planeLocalToWorldTransform);
  }

  auto bounds = calculatePlaneLocalBounds(imageData, planeLocalToWorldTransform);
  if (!bounds || bounds->GetNumberOfPoints() < 2)
  {
    SPDLOG_INFO("plane and image data is not intersect");
    mPrivate->mTrackingStatus = false;
    return mPrivate->mTrackingStatus;
  }

  double origin[3], spacing[3];
  int dimensions[3];
  imageData->GetOrigin(origin);
  imageData->GetSpacing(spacing);
  imageData->GetDimensions(dimensions);
  SPDLOG_INFO("read imageData");

  double planeIndexOrigin[3] = { 0, 0, 0 };
  double* planeWorldOrigin = planeLocalToWorldTransform->TransformDoublePoint(planeIndexOrigin);
  std::shared_ptr<Plane> plane = std::make_shared<Plane>(
    glm::vec3(planeWorldOrigin[0], planeWorldOrigin[2], planeWorldOrigin[1]),
    glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0),
    convertToMat4(planeLocalToWorldTransform->GetMatrix()));

  double minBound[3], maxBound[3];
  bounds->GetPoint(0, minBound);
  bounds->GetPoint(1, maxBound);
  plane->m_Width = maxBound[0] - minBound[0] + 1;
  plane->m_Height = maxBound[1] - minBound[1] + 1;

  if (!mPrivate->mTextureObject)
  {
    mPrivate->mTrackingStatus = false;
    return false;
  }
  mPrivate->mTextureObject->updateBufferSize(plane->m_Width, plane->m_Height);

  SPDLOG_INFO("plane width: {}, height: {}", plane->m_Width, plane->m_Height);

  std::shared_ptr<Volume> volume = std::make_shared<Volume>(
    glm::vec3(origin[0], origin[1], origin[2]), glm::vec3(spacing[0], spacing[1], spacing[2]),
    glm::ivec3(dimensions[0], dimensions[1], dimensions[2]), DataType::FLOAT,
    imageData->GetScalarPointer(), convertToMat4(imageData->GetIndexToPhysicalMatrix()));

  mPrivate->mResliceFilterCuda->setPlane(plane);
  mPrivate->mResliceFilterCuda->setVolume(volume);
  mPrivate->mResliceFilterCuda->setGLTexture(mPrivate->mTextureObject->textureId());
  mPrivate->mResliceFilterCuda->setWindowLevel(4095, 1024);
  mPrivate->mResliceFilterCuda->doFilter();

  SPDLOG_INFO("finished reslice");
  mPrivate->mTrackingStatus = true;
  return mPrivate->mTrackingStatus;
}

vtkSmartPointer<vtkPoints> ImageResliceFilter1::calculatePlaneLocalBounds(
  vtkImageData* volume, vtkTransform* planeLocalToWorldTransform)
{
  // get image data bounds in world and PhysicalToIndexMatrix
  double imageDataBounds[6]{};
  double bounds[6]{ std::numeric_limits<double>::max(), std::numeric_limits<double>::min(),
    std::numeric_limits<double>::max(), std::numeric_limits<double>::min(), 0, 0 };
  volume->GetBounds(imageDataBounds);
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
  if (!planeLocalToWorldTransform || !planeLocalToWorldTransform->GetMatrix())
  {
    std::cout << "transform or matrix get error: PlaneLocalToWorldTransform is nullptr: "
              << (planeLocalToWorldTransform == nullptr)
              << " matrix is nullptr: " << (planeLocalToWorldTransform->GetMatrix() == nullptr)
              << std::endl;
    return nullptr;
  }
  planeWorldToLocalMatrix->DeepCopy(planeLocalToWorldTransform->GetMatrix());
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
    return nullptr;
  }
  // index 1 and 2: bounds
  vtkSmartPointer<vtkPoints> boundPoints = vtkSmartPointer<vtkPoints>::New();
  boundPoints->InsertNextPoint(bounds[0], bounds[2], bounds[4]);
  boundPoints->InsertNextPoint(bounds[1], bounds[3], bounds[5]);

  return boundPoints;
}

int ImageResliceFilter1::calculateMaxTextureSize(vtkImageData* volume)
{
  // calculate max texture size
  int dimensions[3];
  volume->GetDimensions(dimensions);
  int maxDiagonalLength2 = std::max({ dimensions[0] * dimensions[0], dimensions[1] * dimensions[1],
    dimensions[2] * dimensions[2] });
  int textureMaxSize = std::sqrt(maxDiagonalLength2);
  return textureMaxSize;
}

bool ImageResliceFilter1::LineIntersectZero(vtkPoints* points, int p1, int p2, double* bounds)
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