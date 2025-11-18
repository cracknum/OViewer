#ifndef DICOM_READ_FILTER_H
#define DICOM_READ_FILTER_H
#include "Macros.h"
#include <itkImageSource.h>

template <typename OutputImageType>
class DicomReadFilter final : public itk::ImageSource<OutputImageType>
{
public:
  itkTypeMacro(DicomReadFilter, itk::ImageSource<OutputImageType>)
    itkSetPointerDeclare(DicomReadFilter);
  itkFactorylessNewMacro(Self)

    using PixelType = typename OutputImageType::PixelType;
  static constexpr unsigned int ImageDimension = OutputImageType::ImageDimension;

  static_assert(ImageDimension == 3, "DicomSeriesReader only supports 3D output image.");

itkSetMacro(DicomDirectory, std::string)

  protected : DicomReadFilter() = default;
  ~DicomReadFilter() override = default;

  void GenerateData() override;

private:
  std::string m_DicomDirectory;
};

#include "DicomReadFilter.ipp"

#endif // DICOMREADFILTER_H