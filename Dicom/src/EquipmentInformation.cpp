#include "EquipmentInformation.hpp"
#include <itkMetaDataDictionary.h>
#include <itkImageSeriesReader.h>
#include <itkGDCMSeriesFileNames.h>
#include "DicomTags.h"
void EquipmentInformation::parseInfo(const itk::MetaDataDictionary& metaData)
{
  m_Manufacturer = parseTag(metaData, DicomTags::Manufacturer);
  m_ModelName = parseTag(metaData, DicomTags::ManufacturerModelName);
  m_SliceThickness = parseTag(metaData, DicomTags::SliceThickness);
  m_SpacingBetweenSlices = parseTag(metaData, DicomTags::SpacingBetweenSlices);
  m_ProtocolName = parseTag(metaData, DicomTags::ProtocolName);
  m_EchoTime = parseTag(metaData, DicomTags::EchoTime);
  m_RepetitionTime = parseTag(metaData, DicomTags::RepetitionTime);
  m_XRayTubeCurrent = parseTag(metaData, DicomTags::XRayTubeCurrent);
}