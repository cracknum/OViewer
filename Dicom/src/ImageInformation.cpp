#include "DicomTags.h"
#include "ImageInformation.hpp"
#include <gdcmImageHelper.h>
#include <itkVTKImageExport.h>
#include <QString>
#include <qstringlist.h>
#include <spdlog/spdlog.h>
#include <vtkMath.h>
#include <vtkMatrix3x3.h>

vtkSmartPointer<vtkImageData> ImageInformation::GetVtkVolume()
{
  return m_VtkImage;
}

bool ImageInformation::isInstanceUidEmpty() const
{
  return m_InstanceUID.empty();
}

void ImageInformation::setVolumeDirectionMatrix(vtkImageData* imageData)
{
  if (!imageData)
  {
    SPDLOG_ERROR("imageData is nullptr");
    return;
  }

  imageData->SetDirectionMatrix(mDirectionMatrix);
}

ImageInformation::ImageInformation()
  : m_Volume(nullptr)
{
}

void ImageInformation::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << indent << "InstanceUID" << m_InstanceUID << std::endl
     << "rows: " << m_Rows << std::endl
     << "columns: " << m_Columns << std::endl
     << "rowPixelSpacing: " << m_RowPixelSpacing << std::endl
     << "columnPixelSpacing: " << m_ColumnPixelSpacing << std::endl
     << "bitsAllocated: " << m_BitsAllocated << std::endl
     << "pixelRepresentation: " << m_PixelRepresentation << std::endl
     << "imageType: " << m_ImageType << std::endl
     << "windowCenter: " << m_WindowCenter << std::endl
     << "windowWidth: " << m_WindowWidth << std::endl
     << "samplesPerPixel: " << m_SamplesPerPixel << std::endl
     << "photoMetricInterpretation: " << m_PhotoMetricInterpretation << std::endl
     << "rows: " << m_Rows << std::endl;
  if (mDirectionMatrix)
  {
    os << "image information:" << std::endl;
    m_VtkImage->Print(os);
  }
}

inline void ImageInformation::parseInfo(const itk::MetaDataDictionary& metaData)
{
  m_InstanceUID = parseTag(metaData, DicomTags::SeriesInstanceUID);
  m_Rows = parseTag(metaData, DicomTags::Rows);
  m_Columns = parseTag(metaData, DicomTags::Columns);
  const auto spacings = parseTag(metaData, DicomTags::PixelSpacing);
  const auto qSpacings = QString::fromStdString(spacings);
  const auto list = qSpacings.split("\\");

  if (list.size() == 2)
  {
    m_RowPixelSpacing = list.at(0).toStdString();
    m_ColumnPixelSpacing = list.at(1).toStdString();
  }
  else
  {
    m_RowPixelSpacing = "0.0";
    m_ColumnPixelSpacing = "0.0";
  }

  m_BitsAllocated = parseTag(metaData, DicomTags::BitsAllocated);
  m_PixelRepresentation = parseTag(metaData, DicomTags::PixelRepresentation);
  m_ImageType = parseTag(metaData, DicomTags::ImageType);
  m_WindowCenter = parseTag(metaData, DicomTags::WindowCenter);
  m_WindowWidth = parseTag(metaData, DicomTags::WindowWidth);
  m_SamplesPerPixel = parseTag(metaData, DicomTags::SamplesPerPixel);
  m_PhotoMetricInterpretation = parseTag(metaData, DicomTags::PhotometricInterpretation);
  auto directionMatrixStr = parseTag(metaData, DicomTags::ImageOrientationPatient);

  auto directionMatrix = vtkSmartPointer<vtkMatrix3x3>::New();
  if (directionMatrixStr.empty())
  {
    directionMatrix->Identity();
  }
  else
  {
    auto qstr = QString::fromStdString(directionMatrixStr);
    auto directions = qstr.split("\\");
    double xAxis[3] = { directions.at(0).toDouble(), directions.at(1).toDouble(),
      directions.at(2).toDouble() };
    double yAxis[3] = { directions.at(3).toDouble(), directions.at(4).toDouble(),
      directions.at(5).toDouble() };
    double zAxis[3] = {};
    vtkMath::Cross(xAxis, yAxis, zAxis);
    std::copy(xAxis, xAxis + 3, directionMatrix->GetData());
    std::copy(yAxis, yAxis + 3, directionMatrix->GetData() + 3);
    std::copy(zAxis, zAxis + 3, directionMatrix->GetData() + 6);
  }

  mDirectionMatrix = directionMatrix;
}
