#include "PlaneGeometry.h"
#include "PlaneType.h"
#include "SlicedGeometry.h"
#include <gtest/gtest.h>
#include <vtkImageData.h>
#include <vtkMatrix3x3.h>
#include <vtkSmartPointer.h>

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

  auto sagittalSlicedGeometry = vtkSmartPointer<SlicedGeometry>::New();
  sagittalSlicedGeometry->initialize(mImageData, StandardPlane::Sagittal);
  auto sagittalPlaneGeometry = sagittalSlicedGeometry->getPlaneGeometry(0);

  auto sagittalPlaneNormal = sagittalPlaneGeometry->getNormal();
  EXPECT_DOUBLE_EQ(sagittalPlaneNormal[0], 1.0);
  EXPECT_DOUBLE_EQ(sagittalPlaneNormal[1], 0.0);
  EXPECT_DOUBLE_EQ(sagittalPlaneNormal[2], 0.0);

  numberOfSlices = sagittalSlicedGeometry->getNumberOfSlices();
  xExtent = sagittalSlicedGeometry->getExtent(BaseGeometry::Axis::X);
  yExtent = sagittalSlicedGeometry->getExtent(BaseGeometry::Axis::Y);
  zExtent = sagittalSlicedGeometry->getExtent(BaseGeometry::Axis::Z);
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

  auto coronalSlicedGeometry = vtkSmartPointer<SlicedGeometry>::New();
  coronalSlicedGeometry->initialize(mImageData, StandardPlane::Coronal);
  auto coronalPlaneGeometry = coronalSlicedGeometry->getPlaneGeometry(0);

  auto coronalPlaneNormal = coronalPlaneGeometry->getNormal();
  EXPECT_DOUBLE_EQ(coronalPlaneNormal[0], 0.0);
  EXPECT_DOUBLE_EQ(coronalPlaneNormal[1], -1.0);
  EXPECT_DOUBLE_EQ(coronalPlaneNormal[2], 0.0);

  numberOfSlices = coronalSlicedGeometry->getNumberOfSlices();
  xExtent = coronalSlicedGeometry->getExtent(BaseGeometry::Axis::X);
  yExtent = coronalSlicedGeometry->getExtent(BaseGeometry::Axis::Y);
  zExtent = coronalSlicedGeometry->getExtent(BaseGeometry::Axis::Z);
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
