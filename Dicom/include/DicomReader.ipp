#ifndef DICOM_READ_FILTER_IPP
#define DICOM_READ_FILTER_IPP
#include "DicomReader.hpp"
#include "DicomSeries.h"
#include "ImageInformation.hpp"
#include <itkGDCMImageIO.h>
#include <itkGDCMSeriesFileNames.h>
#include <itkImageSeriesReader.h>
#include <omp.h>

template <typename OutputImageType>
DicomReadReader<OutputImageType>::DicomReadReader() = default;
template <typename OutputImageType>
itk::SmartPointer<DicomSeries> DicomReadReader<OutputImageType>::parseSeries(
  const OutputImageType* image) const
{
  itk::Image<short, 3> imagei;
  auto& metaData = imagei.GetMetaDataDictionary();
  metaData
}
template <typename OutputImageType>
void DicomReadReader<OutputImageType>::GenerateData()
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

  m_Series.resize(seriesUIDs.size());
  std::vector<std::string> errors(seriesUIDs.size());

#pragma omp parallel for
  for (int i = 0; i < seriesUIDs.size(); ++i)
  {
    auto seriesId = seriesUIDs.at(i);
    auto filenames = nameGenerator->GetFileNames(seriesId);
    auto reader = ReaderType::New();
    reader->SetFileNames(filenames);
    reader->SetImageIO(itk::GDCMImageIO::New());

    try
    {
      reader->Update();
      OutputImageType::Pointer image = reader->GetOutput();
      itk::SmartPointer<DicomSeries> dicomSeries = m_Series.at(i);
      dicomSeries->parseInfo(image->GetMetaDataDirectory());
      dicomSeries->GetImageInfo()->SetVolume(image);
    }
    catch (itk::ExceptionObject& e)
    {
      errors[i] = std::string("failed to read dicom series: ") + e.what();
    }
  }

  for (int i = 0; i < errors.size(); ++i)
  {
    const auto& err = errors.at(i);
    if (!err.empty())
    {
      itk::ExceptionObject("series " << i << " read error" << err);
    }
  }
}

#endif // DICOM_READ_FILTER_IPP
