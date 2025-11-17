#include "DicomSeries.h"
DicomSeries::DicomSeries() {}
DicomSeries::~DicomSeries() {}
EquipmentInformation* DicomSeries::getEquipmentInfo() const {}
PatientInformation* DicomSeries::getPatient() const {}
SeriesInformation* DicomSeries::getSeriesInfo() const {}
StudyInformation* DicomSeries::getStudyInfo() const {}
std::vector<ImageInformation*> DicomSeries::getImages() const {}
void DicomSeries::addImage(ImageInformation* image) {}