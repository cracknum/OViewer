#ifndef SERIES_SELECTOR_DIALOG_h
#define SERIES_SELECTOR_DIALOG_h
#include "Widget.h"
#include "WidgetsExport.h"
#include <itkSmartPointer.h>
#include <memory>

class DicomSeries;
class WIDGETS_API SeriesSelectDialog final : public Widget
{
public:
  explicit SeriesSelectDialog(const char* widgetName, int widgetFlags = 0);
  ~SeriesSelectDialog() override;
  bool render() override;
  void resize(int width, int height) override;
  void setDicomSeries(const std::vector<itk::SmartPointer<DicomSeries>>& dicomSeries);

private:
  struct Private;
  std::unique_ptr<Private> mPrivate;
};
#endif // SERIES_SELECTOR_DIALOG_h
