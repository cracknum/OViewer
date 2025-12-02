#include "ImageSeriesTableView.h"
#include "DicomSeries.h"
#include <QHeaderView>
#include <QStandardItem>
#include <spdlog/spdlog.h>

enum class TableColumn
{
	UID,
	DIMENSIONS
};

struct ImageSeriesTable::Impl final
{
  ImageSeriesVector m_ImageSeriesVector;
};
ImageSeriesTable::ImageSeriesTable(QWidget* parent)
  : QTableWidget(parent)
{
  m_Impl = std::make_unique<Impl>();
  setColumnCount(2);
  QStringList labels;
  labels << "uid" << "dimensions";
  setHorizontalHeaderLabels(labels);
  verticalHeader()->hide();

  setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);

  connect(this, &QTableWidget::cellClicked, this, &ImageSeriesTable::slotRowSelected);

}

void ImageSeriesTable::slotRowSelected(int row, int column)
{
	auto* item = itemAt(row, static_cast<int>(TableColumn::UID));
	SPDLOG_INFO("image selected: {}", item->text().toStdString());
	emit signalSelectedSeries(item->text());
}

ImageSeriesTable::~ImageSeriesTable() {}

void ImageSeriesTable::addRow(const std::string& seriesId, const ImageInformation* imageInfo)
{
  auto dimensions = imageInfo->GetDimensions();
  auto dimsStr = QString::number(dimensions.at(0)) + "x" + QString::number(dimensions.at(1)) + "x" +
    QString::number(dimensions.at(2));

  int newRow = rowCount();
  insertRow(newRow);
  setItem(newRow, 0, new QTableWidgetItem(QString::fromStdString(seriesId)));
  setItem(newRow, 1, new QTableWidgetItem(dimsStr));
  SPDLOG_DEBUG(
    "imageUID:{0}, dims:{1}, rowCount:{2}", seriesId, dimsStr.toStdString(), rowCount());
}
