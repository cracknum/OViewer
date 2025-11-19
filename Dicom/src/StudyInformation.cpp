#include "StudyInformation.hpp"
#include "DicomTags.h"
#include <itkMetaDataDictionary.h>

void StudyInformation::parseInfo(const itk::MetaDataDictionary& metaData)
{
  m_InstanceUID = parseTag(metaData, DicomTags::StudyInstanceUID);
  m_Date = parseTag(metaData, DicomTags::StudyDate);
  m_Time = parseTag(metaData, DicomTags::StudyTime);
  m_Description = parseTag(metaData, DicomTags::StudyDescription);
  m_Id = parseTag(metaData, DicomTags::StudyID);
}