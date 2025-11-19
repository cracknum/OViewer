#include "DicomReader.hpp"
#include <gtest/gtest.h>

TEST(DicomTest, testDicomReader)
{
  using Image = itk::Image<unsigned char, 3>;

  using ucharDicomReadReader = DicomReadReader<Image>;
  ucharDicomReadReader::Pointer reader = ucharDicomReadReader::New();
  // reader->SetDicomDirectory("D:/Workspace/Data/case2");
  reader->SetDicomDirectory("F:/Workspace/Data/Covid Scans");
  try
  {
    reader->GenerateData();
  }
  catch (itk::ExceptionObject& e)
  {
    std::cout << "error: " << e.what() << std::endl;
  }

  EXPECT_EQ(std::distance(reader->const_begin(), reader->const_end()), 6);
  const auto it = reader->const_begin();
  auto imageInfo = (*it)->GetImageInfo();
  auto size = imageInfo->GetDimensions();
  EXPECT_EQ(size.at(0), 512);
  EXPECT_EQ(size.at(1), 512);
  EXPECT_EQ(size.at(2), 1);
}
