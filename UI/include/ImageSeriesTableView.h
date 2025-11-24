#ifndef IMAGE_SERIES_TABLE
#define IMAGE_SERIES_TABLE
#include <QTableWidget>
#include <memory>
#include <vector>
#include "UIExport.h"

class ImageInformation;
class UI_API ImageSeriesTable final : public QTableWidget
{
public:
	explicit ImageSeriesTable(QWidget* parent = nullptr);
	~ImageSeriesTable();
	using ImageSeriesVector = std::vector<const ImageInformation*>;
	void setImageSeries(const ImageSeriesVector& seriesVector);
	void addRow(const ImageInformation* imageInfo);
private:
	struct Impl;
	std::unique_ptr<Impl> m_Impl;
};
#endif // IMAGE_SERIES_TABLE
