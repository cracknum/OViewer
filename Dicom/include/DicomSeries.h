#ifndef DICOM_DATA_H
#define DICOM_DATA_H
#include <itkObject.h>
#include <vector>
#include "Macros.h"
#include "InformationParser.h"

struct EquipmentInformation;
struct ImageInformation;
struct PatientInformation;
struct SeriesInformation;
struct StudyInformation;

class DicomSeries: public InformationParser
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
  DicomSeries(){}
  ~DicomSeries() override{}

private:
  itk::SmartPointer<EquipmentInformation> m_EquipInfo;
  itk::SmartPointer<PatientInformation> m_PatientInfo;
  itk::SmartPointer<SeriesInformation> m_SeriesInfo;
  itk::SmartPointer<StudyInformation> m_StudyInfo;
  itk::SmartPointer<ImageInformation> m_ImageInfo;
};

#endif //DICOM_DATA_H
