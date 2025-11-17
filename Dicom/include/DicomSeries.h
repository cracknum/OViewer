#ifndef DICOM_DATA_H
#define DICOM_DATA_H
#include <memory>
#include <vector>
struct EquipmentInformation;
struct ImageInformation;
struct PatientInformation;
struct SeriesInformation;
struct StudyInformation;

class DicomSeries
{
public:
  DicomSeries();
  ~DicomSeries();

  EquipmentInformation* getEquipmentInfo() const;
  PatientInformation* getPatient() const;
  SeriesInformation* getSeriesInfo() const;
  StudyInformation* getStudyInfo() const;

  std::vector<ImageInformation*> getImages() const;

  void addImage(ImageInformation* image);

private:
  struct Impl;
  std::unique_ptr<Impl> mImpl;
};

#endif //DICOM_DATA_H
