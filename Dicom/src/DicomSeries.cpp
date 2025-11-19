#include "DicomSeries.h"
#include "DicomTags.h"
#include "EquipmentInformation.hpp"
#include "ImageInformation.hpp"
#include "PatientInformation.hpp"
#include "SeriesInformation.hpp"
#include "StudyInformation.hpp"
#include <itkMetaDataDictionary.h>

void DicomSeries::parseInfo(const itk::MetaDataDictionary& metaData)
{
  m_EquipInfo->parseInfo(metaData);
  m_PatientInfo->parseInfo(metaData);
  m_SeriesInfo->parseInfo(metaData);
  m_StudyInfo->parseInfo(metaData);
  m_ImageInfo->parseInfo(metaData);
}