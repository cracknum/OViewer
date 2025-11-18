#ifndef DICOM_READ_FILTER_IPP
#define DICOM_READ_FILTER_IPP
#include "DicomReadFilter.hpp"
#include <itkGDCMImageIO.h>
#include <itkGDCMSeriesFileNames.h>
#include <itkImageSeriesReader.h>

template <typename OutputImageType>
inline void DicomReadFilter<OutputImageType>::GenerateData()
{
  if (m_DicomDirectory.empty())
  {
    itkExceptionMacro("dicom directory is not set");
  }

  using SeriesNameGeneratorType = itk::GDCMSeriesFileNames;
  using ReaderType = itk::ImageSeriesReader<OutputImageType>;
  auto nameGenerator = SeriesNameGeneratorType::New();
  nameGenerator->SetUseSeriesDetails(true);
  nameGenerator->SetDirectory(m_DicomDirectory);

  const auto seriesUIDs = nameGenerator->GetSeriesUIDs();
  if (seriesUIDs.empty())
  {
    itkExceptionMacro("no dicom series found in directory" << m_DicomDirectory);
  }

  auto seriesIdentifier = seriesUIDs.front();
  auto filenames = nameGenerator->GetFileNames(seriesIdentifier);
  auto reader = ReaderType::New();
  reader->SetFileNames(filenames);
  reader->SetImageIO(itk::GDCMImageIO::New());

  try
  {
    reader->Update();
  }
  catch(itk::ExceptionObject& e)
  {
    itkExceptionMacro("failed to read dicom series: " << e);
  }

  this->GraftOutput(reader->GetOutput());
}


#endif // DICOM_READ_FILTER_IPP
