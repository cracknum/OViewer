#include "DataType.h"
#include "DicomReader.hpp"
#include "DicomSeries.h"
#include "ImageInformation.hpp"
#include "ImageResliceFilterCuda.cuh"
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
#include "ImageResliceFilter.h"
#include <glm/gtx/string_cast.hpp>
#include <spdlog/spdlog.h>
#include <vtkMetaImageWriter.h>
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
  std::unique_ptr<ImageResliceFilterCuda> resliceFilter =
    std::make_unique<ImageResliceFilterCuda>();
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
  reader->SetDicomDirectory("F:/Workspace/Data/Covid Scans");
  reader->GenerateData();

  auto dicomSeries = *(reader->begin() + 1);
  auto imageData = dicomSeries->GetImageInfo()->GetVtkVolume();
  double* origin = imageData->GetOrigin();
  double* spacing = imageData->GetSpacing();
  int* dimensions = imageData->GetDimensions();
  auto uid = dicomSeries->GetSeriesInfo()->GetDescription();
  std::cout << "dimensions: " << dimensions[0] << " " << dimensions[1] << " " << dimensions[2]
            << std::endl;
  vtkSmartPointer<vtkTransform> transform = vtkSmartPointer<vtkTransform>::New();
  vtkSmartPointer<vtkMatrix4x4> planeIndexToWorldMatrix = vtkSmartPointer<vtkMatrix4x4>::New();
  planeIndexToWorldMatrix->Identity();
  planeIndexToWorldMatrix->SetElement(0, 0, spacing[0]);
  planeIndexToWorldMatrix->SetElement(1, 1, spacing[1]);
  planeIndexToWorldMatrix->SetElement(2, 2, spacing[2]);
  planeIndexToWorldMatrix->SetElement(0, 3, origin[0] + 100);
  planeIndexToWorldMatrix->SetElement(1, 3, origin[1] + 90);
  planeIndexToWorldMatrix->SetElement(2, 3, origin[2] + 90);

  vtkNew<vtkMatrix4x4> physicalToIndexMatrix;
  auto* indexToPhySicalMatrix = imageData->GetIndexToPhysicalMatrix();
  physicalToIndexMatrix->DeepCopy(indexToPhySicalMatrix);
  physicalToIndexMatrix->Invert();
  double phyOrigin[4] = { origin[0] + 90, origin[1] + 90.0, origin[2] + 90.0, 1.0 };
  double* indexOrigin = physicalToIndexMatrix->MultiplyDoublePoint(phyOrigin);
  SPDLOG_INFO("index origin: {} {} {} ", indexOrigin[0] / indexOrigin[3],
    indexOrigin[1] / indexOrigin[3], indexOrigin[2] / indexOrigin[3]);
  transform->SetMatrix(planeIndexToWorldMatrix);
  // transform->RotateX(90);
  vtkSmartPointer<ImageResliceFilter> planeLocalBoundsFilter =
    vtkSmartPointer<ImageResliceFilter>::New();
  planeLocalBoundsFilter->SetInputData(imageData);
  planeLocalBoundsFilter->SetPlaneLocalToWorldTransform(transform);
  planeLocalBoundsFilter->Update();
  auto outputImageData = planeLocalBoundsFilter->GetOutput();

  int ouputDimensions[3];
  outputImageData->GetDimensions(ouputDimensions);
  auto pixels = static_cast<const float*>(outputImageData->GetScalarPointer());
  saveFloatBufferAsPNG("test.png", pixels, ouputDimensions[0], ouputDimensions[1]);
}