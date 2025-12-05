#include "DataType.h"
#include "DicomReader.hpp"
#include "DicomSeries.h"
#include "ImageInformation.hpp"
#include "ImageResliceFilter.cuh"
#include "Plane.h"
#include "Volume.h"
#include <glm/glm.hpp>
#include <gtest/gtest.h>
#include <vtkImageData.h>
#include <vtkMatrix3x3.h>
#include <vtkSmartPointer.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#define GLM_ENABLE_EXPERIMENTAL
#include "PlaneLocalBoundsFilter.h"
#include <glm/gtx/string_cast.hpp>
#include <spdlog/spdlog.h>
#include <vtkPointSet.h>

namespace
{
void saveFloatBufferAsPNG(const char* filename, const float* pixels, int width, int height)
{
  // 分配 8-bit RGBA 或 grayscale 缓冲区
  std::vector<unsigned char> out_pixels(width * height);

  // 找 min/max 用于归一化（可选）
  float min_val = *std::min_element(pixels, pixels + width * height);
  float max_val = *std::max_element(pixels, pixels + width * height);
  std::cout << "min_val: " << min_val << " max_val: " << max_val << std::endl;
  float range = max_val - min_val;
  if (range == 0)
    range = 1.0f; // 防止除零

  // 转换 float → uint8
  for (int i = 0; i < width * height; ++i)
  {
    float normalized = (pixels[i] - min_val) / range; // [0, 1]
    normalized = std::clamp(normalized, 0.0f, 1.0f);
    out_pixels[i] = static_cast<unsigned char>(normalized * 255.0f);
  }

  // 写出灰度图（1 channel）
  int err = stbi_write_png(filename, width, height, 1, out_pixels.data(), width);
  if (err == 0)
  {
    const char* failureReason = stbi_failure_reason();
    SPDLOG_ERROR("write failure reason: {}, error code: {}", failureReason, err);
  }
}

glm::mat4 convertTOMat4(const vtkMatrix4x4* matrix)
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

TEST(ImageResliceTest, AxialSliceTest)
{
  GTEST_SKIP() << "skipping AxialSliceTest";
  using ReaderType = DicomReadReader<itk::Image<float, 3>>;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetDicomDirectory("D:/Workspace/Data/case2");
  reader->GenerateData();

  auto dicomSeries = *(reader->begin());
  auto image = dicomSeries->GetImageInfo();
  auto vtkVolume = image->GetVtkVolume();

  const double* origin = vtkVolume->GetOrigin();
  const double* spacing = vtkVolume->GetSpacing();
  const int* dimensions = vtkVolume->GetDimensions();
  vtkMatrix3x3* directionMatrix = vtkVolume->GetDirectionMatrix();
  glm::mat4 matrix(1.0f);
  glm::vec3 gOrigin(origin[0], origin[1], origin[2]);
  glm::vec3 gSpacing(spacing[0], spacing[1], spacing[2]);
  glm::ivec3 gDimensions(dimensions[0], dimensions[1], dimensions[2]);

  matrix[0] = glm::vec4(spacing[0], 0.0f, 0.0f, 0.0f);
  matrix[1] = glm::vec4(0.0f, spacing[1], 0.0f, 0.0f);
  matrix[2] = glm::vec4(0.0f, 0.0f, spacing[2], 0.0f);
  matrix[3] = glm::vec4(gOrigin.x, gOrigin.y, gOrigin.z, 1.0f);
  std::shared_ptr<Volume> volume = std::make_shared<Volume>(gOrigin, gSpacing, gDimensions,
    DataType::FLOAT, static_cast<float*>(vtkVolume->GetScalarPointer()), matrix);
  std::cout << "matrix: " << glm::to_string(matrix) << std::endl;

  glm::mat4 planIndexToWorldMatrix = { glm::vec4(spacing[0], 0.0f, 0.0f, 0.0f),
    glm::vec4(0.0f, spacing[1], 0.0f, 0.0f), glm::vec4(0.0f, 0.0f, spacing[2], 0.0f),
    glm::vec4(gOrigin.x, gOrigin.y, gOrigin.z + 20.0f, 1.0f) };
  std::shared_ptr<Plane> plane =
    std::make_shared<Plane>(glm::vec3(gOrigin.x, gOrigin.y, gOrigin.z + 100.0f), glm::vec3(1, 0, 0),
      glm::vec3(0, 1, 0), planIndexToWorldMatrix);
  plane->m_Width = dimensions[0];
  plane->m_Height = dimensions[1];
  std::cout << glm::to_string(volume->m_WorldToIndex * glm::vec4(plane->m_Origin, 1.0f))
            << std::endl;
  std::unique_ptr<ImageResliceFilter> resliceFilter = std::make_unique<ImageResliceFilter>();
  resliceFilter->setPlane(plane);
  resliceFilter->setVolume(volume);
  resliceFilter->doFilter();
  auto pixels = static_cast<const float*>(resliceFilter->getPixels());
  saveFloatBufferAsPNG("test.png", pixels, dimensions[0], dimensions[1]);
}

TEST(ImageResliceTest, PlaneLocalBoundsFilterTest)
{
  // GTEST_SKIP() << "skipping PlaneLocalBoundsFilterTest";
  using ReaderType = DicomReadReader<itk::Image<float, 3>>;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetDicomDirectory("D:/Workspace/Data/case2");
  reader->GenerateData();

  auto dicomSeries = *(reader->begin());
  auto imageData = dicomSeries->GetImageInfo()->GetVtkVolume();
  double* origin = imageData->GetOrigin();
  double* spacing = imageData->GetSpacing();
  int* dimensions = imageData->GetDimensions();

  vtkSmartPointer<vtkTransform> transform = vtkSmartPointer<vtkTransform>::New();
  vtkSmartPointer<vtkMatrix4x4> matrix = vtkSmartPointer<vtkMatrix4x4>::New();
  matrix->Identity();
  matrix->SetElement(0, 0, spacing[0]);
  matrix->SetElement(1, 1, spacing[1]);
  matrix->SetElement(2, 2, spacing[2]);
  matrix->SetElement(0, 3, origin[0]);
  matrix->SetElement(1, 3, origin[1]);
  matrix->SetElement(2, 3, origin[2] + 20.0);

  transform->SetMatrix(matrix);
  vtkSmartPointer<PlaneLocalBoundsFilter> planeLocalBoundsFilter =
    vtkSmartPointer<PlaneLocalBoundsFilter>::New();
  planeLocalBoundsFilter->SetInputData(imageData);
  planeLocalBoundsFilter->SetPlaneLocalToWorldTransform(transform);
  planeLocalBoundsFilter->Update();
  vtkSmartPointer<vtkPointSet> pointset = planeLocalBoundsFilter->GetOutput();

  EXPECT_EQ(pointset->GetNumberOfPoints(), 3);
  double* textureMaxSize = pointset->GetPoint(0);
  SPDLOG_INFO("textureMaxSize: {} {} {}", textureMaxSize[0], textureMaxSize[1], textureMaxSize[2]);
  double minBound[3];
  pointset->GetPoint(1, minBound);

  SPDLOG_INFO("minBound: {} {} {}", minBound[0], minBound[1], minBound[2]);
  double maxBound[3];
  pointset->GetPoint(2, maxBound);
  SPDLOG_INFO("maxBound: {} {} {}", maxBound[0], maxBound[1], maxBound[2]);

  vtkSmartPointer<PlaneLocalBoundsFilter> planeLocalBoundsFilter1 =
    vtkSmartPointer<PlaneLocalBoundsFilter>::New();

  vtkSmartPointer<vtkMatrix4x4> failPlaneMatrix = vtkSmartPointer<vtkMatrix4x4>::New();
  failPlaneMatrix->Identity();
  transform->SetMatrix(failPlaneMatrix);
  planeLocalBoundsFilter1->SetInputData(imageData);
  planeLocalBoundsFilter1->SetPlaneLocalToWorldTransform(transform);
  planeLocalBoundsFilter1->Update();
  auto pointset1 = planeLocalBoundsFilter1->GetOutput();
  EXPECT_EQ(pointset1->GetNumberOfPoints(), 1); // at least has a texture max size

  auto imageIndexToPhysicalMatrix = imageData->GetIndexToPhysicalMatrix();

  glm::vec3 gOrigin(origin[0], origin[1], origin[2]);
  glm::vec3 gSpacing(spacing[0], spacing[1], spacing[2]);
  glm::ivec3 gDimensions(dimensions[0], dimensions[1], dimensions[2]);
  std::shared_ptr<Volume> volume = std::make_shared<Volume>(gOrigin, gSpacing, gDimensions,
    DataType::FLOAT, static_cast<float*>(imageData->GetScalarPointer()),
    convertTOMat4(imageIndexToPhysicalMatrix));

  auto mat4 = convertTOMat4(imageIndexToPhysicalMatrix);
  SPDLOG_INFO("image data index to world matrix: {}", glm::to_string(mat4));

  std::shared_ptr<Plane> plane =
    std::make_shared<Plane>(glm::vec3(gOrigin.x, gOrigin.y, gOrigin.z + 100.0f), glm::vec3(1, 0, 0),
      glm::vec3(0, 1, 0), convertTOMat4(matrix));
  plane->m_Width = maxBound[0] - minBound[0] + 1;
  plane->m_Height = maxBound[1] - minBound[1] + 1;

  SPDLOG_INFO("plane size: {} x {}", plane->m_Width, plane->m_Height);

  std::unique_ptr<ImageResliceFilter> resliceFilter = std::make_unique<ImageResliceFilter>();
  resliceFilter->setPlane(plane);
  resliceFilter->setVolume(volume);
  resliceFilter->doFilter();
  auto pixels = static_cast<const float*>(resliceFilter->getPixels());
  saveFloatBufferAsPNG("test.png", pixels, plane->m_Width, plane->m_Height);
}