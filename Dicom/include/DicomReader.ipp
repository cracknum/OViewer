#ifndef DICOM_READ_FILTER_IPP
#define DICOM_READ_FILTER_IPP
#include "DicomReader.hpp"
#include "DicomSeries.h"
#include "ImageInformation.hpp"
#include <itkGDCMImageIO.h>
#include <itkGDCMSeriesFileNames.h>
#include <itkImageSeriesReader.h>
#include <itkImage.h>

template <typename OutputImageType>
DicomReadReader<OutputImageType>::DicomReadReader() = default;

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
	auto gdcmIO = itk::GDCMImageIO::New();
    reader->SetImageIO(gdcmIO);

    try
    {
      reader->Update();
      OutputImageType::Pointer image = reader->GetOutput();
      m_Series[i] = DicomSeries::New();
      itk::SmartPointer<DicomSeries> dicomSeries = m_Series.at(i);
      dicomSeries->parseInfo(gdcmIO->GetMetaDataDictionary());
      dicomSeries->GetImageInfo()->SetVolume<OutputImageType::PixelType>(image);
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
      itkExceptionMacro("series " << std::to_string(i).c_str() << " read error" << err);
    }
  }
}

#endif // DICOM_READ_FILTER_IPP
