#include "SeriesInformation.hpp"
#include "DicomTags.h"
#include <itkMetaDataDictionary.h>

void SeriesInformation::parseInfo(const itk::MetaDataDictionary& metaData)
{
  m_InstanceUID = parseTag(metaData, DicomTags::SeriesInstanceUID);
  m_Modality = parseTag(metaData, DicomTags::Modality);
  m_Description = parseTag(metaData, DicomTags::SeriesDescription);
  if (m_Description.empty())
  {
    m_Description = parseTag(metaData, DicomTags::BodyPartExamined);
    if (m_Description.empty())
    {
      m_Description = parseTag(metaData, DicomTags::ProtocolName);
    }
  }
  m_Number = parseTag(metaData, DicomTags::SeriesNumber);
}
void SeriesInformation::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  InformationParser::PrintSelf(os, indent);
  os << indent << "InstanceUID: " << m_InstanceUID << std::endl
     << "Modality: " << m_Modality << std::endl
     << "Description: " << m_Description << std::endl
     << "Number: " << m_Number << std::endl;
}