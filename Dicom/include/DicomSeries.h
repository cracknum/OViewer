#ifndef DICOM_DATA_H
#define DICOM_DATA_H
#include "EquipmentInformation.hpp"
#include "ImageInformation.hpp"
#include "InformationParser.h"
#include "Macros.h"
#include "PatientInformation.hpp"
#include "SeriesInformation.hpp"
#include "StudyInformation.hpp"
#include <itkObject.h>
#include "DicomExport.h"

class DICOM_API DicomSeries : public InformationParser
{
public:
  itkTypeMacro(DicomSeries, InformationParser);
  itkSetPointerDeclare(DicomSeries);
  itkFactorylessNewMacro(Self);
  ITK_DISALLOW_COPY_AND_MOVE(Self);

  itkGetConstMacro(EquipInfo, EquipmentInformation*);
  itkGetConstMacro(PatientInfo, PatientInformation*);
  itkGetConstMacro(SeriesInfo, SeriesInformation*);
  itkGetConstMacro(StudyInfo, StudyInformation*);
  itkGetMacro(ImageInfo, ImageInformation*);

  void parseInfo(const itk::MetaDataDictionary& metaData) override;

protected:
  DicomSeries();
  ~DicomSeries() override = default;

private:
  itk::SmartPointer<EquipmentInformation> m_EquipInfo;
  itk::SmartPointer<PatientInformation> m_PatientInfo;
  itk::SmartPointer<SeriesInformation> m_SeriesInfo;
  itk::SmartPointer<StudyInformation> m_StudyInfo;
  itk::SmartPointer<ImageInformation> m_ImageInfo;
};

#endif // DICOM_DATA_H
