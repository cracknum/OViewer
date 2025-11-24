#ifndef DICOM_READ_FILTER_IPP
#define DICOM_READ_FILTER_IPP
#include "DicomReader.hpp"
#include "DicomSeries.h"
#include "ImageInformation.hpp"
#include <itkGDCMImageIO.h>
#include <itkGDCMSeriesFileNames.h>
#include <itkImageSeriesReader.h>
#include <itkImage.h>

DicomReadReader::DicomReadReader() = default;

void DicomReadReader::GenerateData()
{
	if (m_DicomDirectory.empty())
	{
		itkExceptionMacro("dicom directory is not set");
	}

	using SeriesNameGeneratorType = itk::GDCMSeriesFileNames;
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

		auto gdcmIO = itk::GDCMImageIO::New();
		gdcmIO->SetFileName(filenames.front());
		gdcmIO->ReadImageInformation();
		itk::ImageIOBase::IOComponentEnum componentType = gdcmIO->GetComponentType();
		m_Series[i] = DicomSeries::New();
		itk::SmartPointer<DicomSeries> dicomSeries = m_Series.at(i);
		errors[i] = dispatchRead(componentType, filenames, dicomSeries.get());

		for (int i = 0; i < errors.size(); ++i)
		{
			const auto& err = errors.at(i);
			if (!err.empty())
			{
				itkExceptionMacro("series " << std::to_string(i).c_str() << " read error" << err);
			}
		}

	}
}

template<itk::ImageIOBase::IOComponentType VComponent>
struct ComponentToCxxType;

template<> struct ComponentToCxxType<itk::ImageIOBase::UCHAR> { using Type = unsigned char; };
template<> struct ComponentToCxxType<itk::ImageIOBase::USHORT> { using Type = unsigned short; };
template<> struct ComponentToCxxType<itk::ImageIOBase::FLOAT> { using Type = float; };
template<> struct ComponentToCxxType<itk::ImageIOBase::SHORT> { using Type = short; };

template<typename TPixel>
std::string DicomReadReader::doReadData(
	const itk::GDCMSeriesFileNames::FileNamesContainerType& fileNames,
	DicomSeries* dicomSeries)
{
	using OutputImageType = itk::Image<TPixel, 3>;
	using ReaderType = itk::ImageSeriesReader<OutputImageType>;
	auto reader = ReaderType::New();
	reader->SetFileNames(fileNames);
	auto gdcmIO = itk::GDCMImageIO::New();
	reader->SetImageIO(gdcmIO);
	gdcmIO->LoadPrivateTagsOn();
	gdcmIO->KeepOriginalUIDOn();

	try
	{
		reader->Update();
		OutputImageType::Pointer image = reader->GetOutput();
		dicomSeries->parseInfo(gdcmIO->GetMetaDataDictionary());
		dicomSeries->GetImageInfo()->SetVolume<TPixel>(image);
	}
	catch (itk::ExceptionObject& e)
	{
		return std::string("failed to read dicom series: ") + e.what();
	}

	return "";
}

std::string DicomReadReader::dispatchRead(
	itk::ImageIOBase::IOComponentType VComponent,
	const itk::GDCMSeriesFileNames::FileNamesContainerType& fileNames,
	DicomSeries* series)
{
	switch (VComponent)
	{
	case itk::ImageIOBase::UCHAR:
		return doReadData<ComponentToCxxType<itk::ImageIOBase::UCHAR>::Type>(fileNames, series);
		break;
	case itk::ImageIOBase::USHORT:
		return doReadData<ComponentToCxxType<itk::ImageIOBase::UCHAR>::Type>(fileNames, series);
		break;
	case itk::ImageIOBase::SHORT:
		return doReadData<ComponentToCxxType<itk::ImageIOBase::SHORT>::Type>(fileNames, series);
		break;
	case itk::ImageIOBase::FLOAT:
		return doReadData<ComponentToCxxType<itk::ImageIOBase::FLOAT>::Type>(fileNames, series);
		break;
	default:
		throw std::runtime_error("not supprt datatype");
		break;
	}
}

#endif // DICOM_READ_FILTER_IPP
