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
  stbi_write_png(filename, width, height, 1, out_pixels.data(), width);
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

  matrix[3] = glm::vec4(gOrigin.x, gOrigin.y, gOrigin.z, 1.0f);
  std::shared_ptr<Volume> volume = std::make_shared<Volume>(gOrigin, gSpacing, gDimensions,
    DataType::FLOAT, static_cast<float*>(vtkVolume->GetScalarPointer()), matrix);
  std::cout << "matrix: " << glm::to_string(matrix) << std::endl;

  std::shared_ptr<Plane> plane =
    std::make_shared<Plane>(glm::vec3(gOrigin.x, gOrigin.y, gOrigin.z + 100.0f), glm::vec3(1, 0, 0),
      glm::vec3(0, 1, 0), matrix);
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
  matrix->SetElement(2, 3, -20);
  transform->SetMatrix(matrix);
  vtkSmartPointer<PlaneLocalBoundsFilter> planeLocalBoundsFilter = vtkSmartPointer<PlaneLocalBoundsFilter>::New();
  planeLocalBoundsFilter->SetInputData(imageData);
  planeLocalBoundsFilter->SetPlaneLocalToWorldTransform(transform);
  planeLocalBoundsFilter->Update();
  vtkSmartPointer<vtkPointSet> pointset = planeLocalBoundsFilter->GetOutput();

  EXPECT_EQ(pointset->GetNumberOfPoints(), 2);

  vtkSmartPointer<PlaneLocalBoundsFilter> planeLocalBoundsFilter1 = vtkSmartPointer<PlaneLocalBoundsFilter>::New();

  matrix->Identity();
  transform->SetMatrix(matrix);
  planeLocalBoundsFilter1->SetInputData(imageData);
  planeLocalBoundsFilter1->SetPlaneLocalToWorldTransform(transform);
  planeLocalBoundsFilter1->Update();
  auto pointset1 = planeLocalBoundsFilter1->GetOutput();
  EXPECT_EQ(pointset1->GetNumberOfPoints(), 0);
}