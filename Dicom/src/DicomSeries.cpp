#include "DicomSeries.h"
#include <itkMetaDataDictionary.h>

void DicomSeries::parseInfo(const itk::MetaDataDictionary& metaData)
{
  m_EquipInfo->parseInfo(metaData);
  m_PatientInfo->parseInfo(metaData);
  m_SeriesInfo->parseInfo(metaData);
  m_StudyInfo->parseInfo(metaData);
  m_ImageInfo->parseInfo(metaData);
}
DicomSeries::DicomSeries()
{
  m_EquipInfo = EquipmentInformation::New();
  m_PatientInfo = PatientInformation::New();
  m_SeriesInfo = SeriesInformation::New();
  m_StudyInfo = StudyInformation::New();
  m_ImageInfo = ImageInformation::New();
}
