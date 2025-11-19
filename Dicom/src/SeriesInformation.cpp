#include "SeriesInformation.hpp"
#include "DicomTags.h"
#include <itkMetaDataDictionary.h>

void SeriesInformation::parseInfo(const itk::MetaDataDictionary& metaData)
{
  m_InstanceUID = parseTag(metaData, DicomTags::SeriesInstanceUID);
  m_Modality = parseTag(metaData, DicomTags::Modality);
  m_Description = parseTag(metaData, DicomTags::SeriesDescription);
  m_Number = parseTag(metaData, DicomTags::SeriesNumber);
}