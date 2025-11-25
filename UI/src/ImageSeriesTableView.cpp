#include "ImageSeriesTableView.h"
#include "DicomSeries.h"
#include <QHeaderView>
#include <QStandardItem>
#include <spdlog/spdlog.h>

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
}

ImageSeriesTable::~ImageSeriesTable() {}

void ImageSeriesTable::setImageSeries(const ImageSeriesVector& seriesVector)
{
  m_Impl->m_ImageSeriesVector = seriesVector;

  for (auto it : seriesVector)
  {
    addRow(it);
  }
}

void ImageSeriesTable::addRow(const ImageInformation* imageInfo)
{
  auto imageUID = imageInfo->GetInstanceUID();
  auto dimensions = imageInfo->GetDimensions();
  auto dimsStr = QString::number(dimensions.at(0)) + "x" + QString::number(dimensions.at(1)) + "x" +
    QString::number(dimensions.at(2));

  int newRow = rowCount();
  insertRow(newRow);
  setItem(newRow, 0, new QTableWidgetItem(QString::fromStdString(imageUID)));
  setItem(newRow, 1, new QTableWidgetItem(dimsStr));
  SPDLOG_DEBUG(
    "imageUID:{0}, dims:{1}, rowCount:{2}", imageUID, dimsStr.toStdString(), rowCount());
}
