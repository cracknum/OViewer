#ifndef IMAGE_SERIES_TABLE
#define IMAGE_SERIES_TABLE
#include "UIExport.h"
#include <QTableWidget>
#include <memory>
#include <vector>

class ImageInformation;
class UI_API ImageSeriesTable final : public QTableWidget
{
	Q_OBJECT
public:
  explicit ImageSeriesTable(QWidget* parent = nullptr);
  ~ImageSeriesTable();
  using ImageSeriesVector = std::vector<const ImageInformation*>;
  void addRow(const std::string& seriesId, const ImageInformation* imageInfo);

public slots:
  void slotRowSelected(int row, int column);
signals:
  void signalSelectedSeries(const QString& seriesId);

private:
  struct Impl;
  std::unique_ptr<Impl> m_Impl;
};
#endif // IMAGE_SERIES_TABLE
