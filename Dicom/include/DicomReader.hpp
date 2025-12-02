#ifndef DICOM_READ_FILTER_H
#define DICOM_READ_FILTER_H
#include "Macros.h"
#include <itkImageSource.h>

class DicomSeries;
template <typename OutputImageType>
class DicomReadReader final : public itk::Object
{
public:
  itkTypeMacro(DicomReadReader, itk::Object);
  itkSetPointerDeclare(DicomReadReader);
  itkFactorylessNewMacro(Self);
  using SeriesVector = std::vector<itk::SmartPointer<DicomSeries>>;
  using PixelType = typename OutputImageType::PixelType;
  static constexpr unsigned int ImageDimension = OutputImageType::ImageDimension;

  static_assert(ImageDimension == 3, "DicomSeriesReader only supports 3D output image.");

  itkSetStringMacro(DicomDirectory);

  void GenerateData();

  SeriesVector::iterator begin() { return m_Series.begin(); }
  SeriesVector::iterator end() { return m_Series.end(); }
  SeriesVector::const_iterator const_begin() const { return m_Series.cbegin(); }
  SeriesVector::const_iterator const_end() const { return m_Series.cend(); }

protected:
  DicomReadReader();
  ~DicomReadReader() override = default;
private:
  std::string m_DicomDirectory;

  SeriesVector m_Series;
};

#include "DicomReader.ipp"

#endif // DICOM_READ_FILTER_H