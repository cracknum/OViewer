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

TEST(ImageResliceTest, AxialSliceTest)
{
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
  matrix[0] = glm::vec4(directionMatrix->GetElement(0, 1), directionMatrix->GetElement(0, 2),
    directionMatrix->GetElement(0, 3), origin[0]);
  matrix[1] = glm::vec4(directionMatrix->GetElement(1, 1), directionMatrix->GetElement(1, 2),
    directionMatrix->GetElement(1, 3), origin[1]);
  matrix[2] = glm::vec4(directionMatrix->GetElement(2, 1), directionMatrix->GetElement(2, 2),
    directionMatrix->GetElement(2, 3), origin[2]);
  glm::vec3 gOrigin(origin[0], origin[1], origin[2]);
  glm::vec3 gSpacing(spacing[0], spacing[1], spacing[2]);
  glm::ivec3 gDimensions(dimensions[0], dimensions[1], dimensions[2]);
  std::shared_ptr<Volume> volume = std::make_shared<Volume>(gOrigin, gSpacing, gDimensions,
    DataType::FLOAT, static_cast<float*>(vtkVolume->GetScalarPointer()), matrix);

  std::shared_ptr<Plane> plane = std::make_shared<Plane>(
    glm::vec3(origin[0], origin[1], origin[2]), glm::vec3(1, 0, 0), glm::vec3(0, 1, 0), matrix);

  std::unique_ptr<ImageResliceFilter> resliceFilter = std::make_unique<ImageResliceFilter>();
  resliceFilter->setPlane(plane);
  resliceFilter->setVolume(volume);
  resliceFilter->doFilter();
  EXPECT_EQ(1, 0);
}