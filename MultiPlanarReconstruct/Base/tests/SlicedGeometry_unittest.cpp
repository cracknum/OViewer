#include "PlaneGeometry.h"
#include "PlaneType.h"
#include "SliceNavigator.h"
#include "SlicedGeometry.h"
#include <gtest/gtest.h>
#include <vtkImageData.h>
#include <vtkMatrix3x3.h>
#include <vtkMatrix4x4.h>
#include <vtkSmartPointer.h>
#include <vtkTransform.h>

class SlicedGeometryFixture : public ::testing::Test
{
protected:
  void SetUp() override
  {
    mImageData = vtkSmartPointer<vtkImageData>::New();
    auto directionMatrix = vtkSmartPointer<vtkMatrix3x3>::New();
    directionMatrix->Identity();
    mImageData->SetDirectionMatrix(directionMatrix);
    mOrigin[0] = 0;
    mOrigin[1] = 0;
    mOrigin[2] = 0;
    mDimensions[0] = 200;
    mDimensions[1] = 300;
    mDimensions[2] = 500;
    mSpacings[0] = 0.2;
    mSpacings[1] = 0.2;
    mSpacings[2] = 0.2;
    mImageData->SetOrigin(mOrigin);
    mImageData->SetDimensions(mDimensions);
    mImageData->SetSpacing(mSpacings);
    mImageData->AllocateScalars(VTK_SHORT, 1);
  }
  void TearDown() override {}

protected:
  vtkSmartPointer<vtkImageData> mImageData;
  double mOrigin[3];
  int mDimensions[3];
  double mSpacings[3];
};

TEST_F(SlicedGeometryFixture, initializeAxialTest)
{
  /**
   * 通过这个单元测试就可以得到每个SlicedGeometry是沿着plane的法向增长的使用和plane相同的slice坐标系，
   * 而不是和数据相同的索引坐标系
   */
  auto axialSlicedGeometry = vtkSmartPointer<SlicedGeometry>::New();
  axialSlicedGeometry->initialize(mImageData, StandardPlane::Axial);

  int numberOfSlices = axialSlicedGeometry->getNumberOfSlices();
  int xExtent = axialSlicedGeometry->getExtent(BaseGeometry::Axis::X);
  int yExtent = axialSlicedGeometry->getExtent(BaseGeometry::Axis::Y);
  int zExtent = axialSlicedGeometry->getExtent(BaseGeometry::Axis::Z);
  double bounds[6];
  axialSlicedGeometry->getBounds(bounds);

  int axialIndex[3] = { 0, 1, 2 };
  EXPECT_EQ(numberOfSlices, mDimensions[2]);
  EXPECT_EQ(xExtent, mDimensions[axialIndex[0]] - 1);
  EXPECT_EQ(yExtent, mDimensions[axialIndex[1]] - 1);
  EXPECT_EQ(zExtent, mDimensions[axialIndex[2]] - 1);
  for (size_t i = 0; i < 3; i++)
  {
    EXPECT_EQ(bounds[2 * i], 0);
    EXPECT_EQ(bounds[2 * i + 1], mDimensions[axialIndex[i]] - 1);
  }
  auto axialPlaneGeometry = axialSlicedGeometry->getPlaneGeometry(0);
  ASSERT_TRUE(axialPlaneGeometry != nullptr);

  auto axialPlaneNormal = axialPlaneGeometry->getNormal();
  EXPECT_DOUBLE_EQ(axialPlaneNormal[0], 0.0);
  EXPECT_DOUBLE_EQ(axialPlaneNormal[1], 0.0);
  EXPECT_DOUBLE_EQ(axialPlaneNormal[2], 1.0);
}

TEST_F(SlicedGeometryFixture, initializeSagittalTest)
{
  double bounds[6];

  auto sagittalSlicedGeometry = vtkSmartPointer<SlicedGeometry>::New();
  sagittalSlicedGeometry->initialize(mImageData, StandardPlane::Sagittal);
  auto sagittalPlaneGeometry = sagittalSlicedGeometry->getPlaneGeometry(0);

  auto sagittalPlaneNormal = sagittalPlaneGeometry->getNormal();
  EXPECT_DOUBLE_EQ(sagittalPlaneNormal[0], 1.0);
  EXPECT_DOUBLE_EQ(sagittalPlaneNormal[1], 0.0);
  EXPECT_DOUBLE_EQ(sagittalPlaneNormal[2], 0.0);

  auto numberOfSlices = sagittalSlicedGeometry->getNumberOfSlices();
  auto xExtent = sagittalSlicedGeometry->getExtent(BaseGeometry::Axis::X);
  auto yExtent = sagittalSlicedGeometry->getExtent(BaseGeometry::Axis::Y);
  auto zExtent = sagittalSlicedGeometry->getExtent(BaseGeometry::Axis::Z);
  sagittalSlicedGeometry->getBounds(bounds);

  int sagittalIndex[3] = { 1, 2, 0 };
  EXPECT_EQ(numberOfSlices, mDimensions[0]);
  EXPECT_EQ(xExtent, mDimensions[sagittalIndex[0]] - 1);
  EXPECT_EQ(yExtent, mDimensions[sagittalIndex[1]] - 1);
  EXPECT_EQ(zExtent, mDimensions[sagittalIndex[2]] - 1);
  for (size_t i = 0; i < 3; i++)
  {
    EXPECT_EQ(bounds[2 * i], 0);
    EXPECT_EQ(bounds[2 * i + 1], mDimensions[sagittalIndex[i]] - 1);
  }
}

TEST_F(SlicedGeometryFixture, initializeCoronalTest)
{
  double bounds[6];

  auto coronalSlicedGeometry = vtkSmartPointer<SlicedGeometry>::New();
  coronalSlicedGeometry->initialize(mImageData, StandardPlane::Coronal);
  auto coronalPlaneGeometry = coronalSlicedGeometry->getPlaneGeometry(0);

  auto coronalPlaneNormal = coronalPlaneGeometry->getNormal();
  EXPECT_DOUBLE_EQ(coronalPlaneNormal[0], 0.0);
  EXPECT_DOUBLE_EQ(coronalPlaneNormal[1], -1.0);
  EXPECT_DOUBLE_EQ(coronalPlaneNormal[2], 0.0);

  auto numberOfSlices = coronalSlicedGeometry->getNumberOfSlices();
  auto xExtent = coronalSlicedGeometry->getExtent(BaseGeometry::Axis::X);
  auto yExtent = coronalSlicedGeometry->getExtent(BaseGeometry::Axis::Y);
  auto zExtent = coronalSlicedGeometry->getExtent(BaseGeometry::Axis::Z);
  coronalSlicedGeometry->getBounds(bounds);

  int coronalIndex[3] = { 0, 2, 1 };
  EXPECT_EQ(numberOfSlices, mDimensions[1]);
  EXPECT_EQ(xExtent, mDimensions[coronalIndex[0]] - 1);
  EXPECT_EQ(yExtent, mDimensions[coronalIndex[1]] - 1);
  EXPECT_EQ(zExtent, mDimensions[coronalIndex[2]] - 1);
  for (size_t i = 0; i < 3; i++)
  {
    EXPECT_EQ(bounds[2 * i], 0);
    EXPECT_EQ(bounds[2 * i + 1], mDimensions[coronalIndex[i]] - 1);
  }
}

TEST_F(SlicedGeometryFixture, AxialSliceNavigatorCreatTest)
{
  auto axialsliceNavigator = vtkSmartPointer<SliceNavigator>::New();
  axialsliceNavigator->setReferenceImage(mImageData, StandardPlane::Axial);
  auto plane = axialsliceNavigator->getCurrentPlaneGeometry();

  ASSERT_TRUE(plane != nullptr);
  axialsliceNavigator->nextSlice();
  auto plane1 = axialsliceNavigator->getCurrentPlaneGeometry();
  ASSERT_TRUE(plane1 != nullptr);
  auto* planeTransform = plane->getIndexToWorldTransform();
  auto* plane1Transform = plane->getIndexToWorldTransform();
  auto* planeMatrix = planeTransform->GetMatrix();
  auto* plane1Matrix = plane1Transform->GetMatrix();
  ASSERT_TRUE(plane != plane1);
  for (size_t i = 0; i < 3; i++)
  {
    for (size_t j = 0; j < 3; j++)
    {
      ASSERT_TRUE(std::abs(planeMatrix->GetElement(i, j) - plane1Matrix->GetElement(i, j)) < 1e-6);
    }
  }
  double planeOrigin[3]{};
  double plane1Origin[3]{};
  double planeSpacing[3]{};
  double plane1Spacing[3]{};
  plane->getOrigin(planeOrigin);
  plane1->getOrigin(plane1Origin);
  plane->getSpacing(planeSpacing);
  plane1->getSpacing(plane1Spacing);
  for (size_t i = 0; i < 3; i++)
  {
    ASSERT_TRUE(std::abs(planeSpacing[i] - plane1Spacing[i]) < 1e-6);
  }
  auto planeNormal = plane->getNormal();
  planeNormal.Normalize();
  for (size_t i = 0; i < 3; i++)
  {
    EXPECT_EQ(planeOrigin[i] + planeNormal.GetData()[i] * planeSpacing[i], plane1Origin[i])
      << "plane1 origin: " << plane1Origin[0] << " " << plane1Origin[1] << " " << plane1Origin[2]
      << "\n"
      << "plane Origin: " << planeOrigin[0] << " " << planeOrigin[1] << " " << planeOrigin[2];
  }

  axialsliceNavigator->previousSlice();
  auto plane2 = axialsliceNavigator->getCurrentPlaneGeometry();
  EXPECT_EQ(plane, plane2);
}

TEST_F(SlicedGeometryFixture, SagittalSliceNavigatorCreatTest)
{
  auto sagtittalsliceNavigator = vtkSmartPointer<SliceNavigator>::New();
  sagtittalsliceNavigator->setReferenceImage(mImageData, StandardPlane::Sagittal);
  auto plane = sagtittalsliceNavigator->getCurrentPlaneGeometry();

  ASSERT_TRUE(plane != nullptr);
  sagtittalsliceNavigator->nextSlice();
  auto plane1 = sagtittalsliceNavigator->getCurrentPlaneGeometry();
  ASSERT_TRUE(plane1 != nullptr);
  auto* planeTransform = plane->getIndexToWorldTransform();
  auto* plane1Transform = plane->getIndexToWorldTransform();
  auto* planeMatrix = planeTransform->GetMatrix();
  auto* plane1Matrix = plane1Transform->GetMatrix();
  ASSERT_TRUE(plane != plane1);
  for (size_t i = 0; i < 3; i++)
  {
    for (size_t j = 0; j < 3; j++)
    {
      ASSERT_TRUE(std::abs(planeMatrix->GetElement(i, j) - plane1Matrix->GetElement(i, j)) < 1e-6);
    }
  }
  double planeOrigin[3]{};
  double plane1Origin[3]{};
  double planeSpacing[3]{};
  double plane1Spacing[3]{};
  plane->getOrigin(planeOrigin);
  plane1->getOrigin(plane1Origin);
  plane->getSpacing(planeSpacing);
  plane1->getSpacing(plane1Spacing);
  for (size_t i = 0; i < 3; i++)
  {
    ASSERT_TRUE(std::abs(planeSpacing[i] - plane1Spacing[i]) < 1e-6);
  }
  auto planeNormal = plane->getNormal();
  planeNormal.Normalize();
  for (size_t i = 0; i < 3; i++)
  {
    EXPECT_EQ(planeOrigin[i] + planeNormal.GetData()[i] * planeSpacing[i], plane1Origin[i])
	  << "plane spacing: " << planeSpacing[0] << " " << planeSpacing[1] << " " << planeSpacing[2] << "\n"
      << "plane1 origin: " << plane1Origin[0] << " " << plane1Origin[1] << " " << plane1Origin[2]
      << "\n"
      << "plane Origin: " << planeOrigin[0] << " " << planeOrigin[1] << " " << planeOrigin[2];
  }

  sagtittalsliceNavigator->previousSlice();
  auto plane2 = sagtittalsliceNavigator->getCurrentPlaneGeometry();
  EXPECT_EQ(plane, plane2);
}

TEST_F(SlicedGeometryFixture, CoronalSliceNavigatorCreatTest)
{
  auto coronalsliceNavigator = vtkSmartPointer<SliceNavigator>::New();
  coronalsliceNavigator->setReferenceImage(mImageData, StandardPlane::Coronal);
  auto plane = coronalsliceNavigator->getCurrentPlaneGeometry();

  ASSERT_TRUE(plane != nullptr);
  coronalsliceNavigator->nextSlice();
  auto plane1 = coronalsliceNavigator->getCurrentPlaneGeometry();
  ASSERT_TRUE(plane1 != nullptr);
  auto* planeTransform = plane->getIndexToWorldTransform();
  auto* plane1Transform = plane->getIndexToWorldTransform();
  auto* planeMatrix = planeTransform->GetMatrix();
  auto* plane1Matrix = plane1Transform->GetMatrix();
  ASSERT_TRUE(plane != plane1);
  for (size_t i = 0; i < 3; i++)
  {
    for (size_t j = 0; j < 3; j++)
    {
      ASSERT_TRUE(std::abs(planeMatrix->GetElement(i, j) - plane1Matrix->GetElement(i, j)) < 1e-6);
    }
  }
  double planeOrigin[3]{};
  double plane1Origin[3]{};
  double planeSpacing[3]{};
  double plane1Spacing[3]{};
  plane->getOrigin(planeOrigin);
  plane1->getOrigin(plane1Origin);
  plane->getSpacing(planeSpacing);
  plane1->getSpacing(plane1Spacing);
  for (size_t i = 0; i < 3; i++)
  {
    ASSERT_TRUE(std::abs(planeSpacing[i] - plane1Spacing[i]) < 1e-6);
  }
  auto planeNormal = plane->getNormal();
  planeNormal.Normalize();
  for (size_t i = 0; i < 3; i++)
  {
    EXPECT_EQ(planeOrigin[i] + planeNormal.GetData()[i] * planeSpacing[i], plane1Origin[i])
      << "plane1 origin: " << plane1Origin[0] << " " << plane1Origin[1] << " " << plane1Origin[2]
      << "\n"
      << "plane Origin: " << planeOrigin[0] << " " << planeOrigin[1] << " " << planeOrigin[2];
  }

  coronalsliceNavigator->previousSlice();
  auto plane2 = coronalsliceNavigator->getCurrentPlaneGeometry();
  EXPECT_EQ(plane, plane2);
}