#include "PatientInformation.hpp"
#include "DicomTags.h"
#include <itkMetaDataDictionary.h>

void PatientInformation::parseInfo(const itk::MetaDataDictionary& metaData)
{
  m_Name = parseTag(metaData, DicomTags::PatientName);
  m_Id = parseTag(metaData, DicomTags::PatientID);
  m_BirthDay = parseTag(metaData, DicomTags::PatientBirthDate);
  m_Sex = parseTag(metaData, DicomTags::PatientSex);
  m_Age = parseTag(metaData, DicomTags::PatientAge);
  m_Size = parseTag(metaData, DicomTags::PatientSize);
  m_Weight = parseTag(metaData, DicomTags::PatientWeight);
}