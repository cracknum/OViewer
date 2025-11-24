#ifndef DICOM_READ_FILTER_H
#define DICOM_READ_FILTER_H
#include "Macros.h"
#include <itkImageSource.h>
#include <itkImageIOBase.h>
#include <itkGDCMSeriesFileNames.h>

class DicomSeries;
class DicomReadReader final : public itk::Object
{
public:
	itkTypeMacro(DicomReadReader, itk::Object);
	itkSetPointerDeclare(DicomReadReader);
	itkFactorylessNewMacro(Self);
	using SeriesVector = std::vector<itk::SmartPointer<DicomSeries>>;

	itkSetMacro(DicomDirectory, std::string);

	void GenerateData();

	SeriesVector::iterator begin() { return m_Series.begin(); }
	SeriesVector::iterator end() { return m_Series.end(); }
	SeriesVector::const_iterator const_begin() const { return m_Series.cbegin(); }
	SeriesVector::const_iterator const_end() const { return m_Series.cend(); }

protected:
	DicomReadReader();
	~DicomReadReader() override = default;

	template <typename TPixel>
	std::string doReadData(const itk::GDCMSeriesFileNames::FileNamesContainerType& fileNames, DicomSeries* dicomSeries);
	std::string dispatchRead(
		itk::ImageIOBase::IOComponentType VComponent, 
		const itk::GDCMSeriesFileNames::FileNamesContainerType& fileNames,
		DicomSeries* dicomSeries);
private:
	std::string m_DicomDirectory;

	SeriesVector m_Series;
};

#include "DicomReader.ipp"

#endif // DICOM_READ_FILTER_H
