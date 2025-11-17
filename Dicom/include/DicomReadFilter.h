#ifndef DICOM_READ_FILTER_H
#define DICOM_READ_FILTER_H
#include <itkImageSource.h>
#include "Macros.h"

template<typename OutputImageType>
class DicomReadFilter final : public itk::ImageSource<OutputImageType> {
public:
  itkTypeMacro(DicomReadFilter, itk::LightObject)
  itkSetPointerDeclare(DicomReadFilter);
  itkFactorylessNewMacro(Self)

  using PixelType = typename OutputImageType::PixelType;
  static constexpr unsigned int ImageDimension = typename OutputImageType::ImageDimension;

  static_assert(ImageDimension == 3, "DicomSeriesReader only supports 3D output image.");

  itkSetMacro(DicomDirectory, std::string)

protected:
  DicomReadFilter();
  ~DicomReadFilter() override;

  void GenerateData() override;

private:
  std::string m_DicomDirectory;

}; // DICOM_READ_FILTER_H



#endif //DICOMREADFILTER_H
