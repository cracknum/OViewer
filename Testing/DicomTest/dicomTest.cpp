#include <gtest/gtest.h>
#include "DicomReader.hpp"

TEST(DicomTest, testDicomReader)
{
    using Image = itk::Image<unsigned char, 3>;
    using ucharDicomReadReader = DicomReadReader<Image>;
    ucharDicomReadReader::Pointer reader = ucharDicomReadReader::New();
    reader->SetDicomDirectory("D:/Workspace/Data/case2");
    reader->GenerateData();

    EXPECT_GT(std::distance(reader->const_begin(), reader->const_end()), 0);
}
