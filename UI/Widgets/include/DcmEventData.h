#ifndef DCM_EVENT_DATA_H
#define DCM_EVENT_DATA_H
#include "DicomSeries.h"
#include "EventData.h"
#include "WidgetsExport.h"

class WIDGETS_API SeriesSelectedData final : public EventData
{
public:
  explicit SeriesSelectedData(itk::SmartPointer<DicomSeries> dicomSeries)
    : mDicomSeries(std::move(dicomSeries))
  {
  }

  [[nodiscard]] itk::SmartPointer<DicomSeries> dicomSeries() const { return mDicomSeries; }

private:
  itk::SmartPointer<DicomSeries> mDicomSeries;
};

#endif // DCM_EVENT_DATA_H
