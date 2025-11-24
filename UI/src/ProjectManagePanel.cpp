#include "ProjectManagePanel.h"
#include "DicomSeries.h"
#include "ImageSeriesTableView.h"
#include <QGridLayout>
#include <QTabWidget>

struct ProjectManagePanel::Impl
{
  QTabWidget* m_TabWidget;
  ImageSeriesTable* m_ImageSeriesTable;

  Impl()
    : m_TabWidget(nullptr)
    , m_ImageSeriesTable(nullptr)
  {
  }
};

ProjectManagePanel::ProjectManagePanel(QWidget* parent)
  : QWidget(parent)
{
  initParams();
  initUI();
  initConnects();
}

ProjectManagePanel::~ProjectManagePanel() {}

void ProjectManagePanel::initUI()
{
  m_Impl->m_TabWidget = new QTabWidget();
  QGridLayout* gridLayout = new QGridLayout();
  gridLayout->addWidget(m_Impl->m_TabWidget);
  setLayout(gridLayout);

  m_Impl->m_ImageSeriesTable = new ImageSeriesTable();
  m_Impl->m_TabWidget->addTab(m_Impl->m_ImageSeriesTable, "Images");
}

void ProjectManagePanel::initConnects() {}

void ProjectManagePanel::initParams()
{
  m_Impl = std::make_unique<Impl>();
}

void ProjectManagePanel::slotSetImageTable(const SeriesVector& seriesVector)
{
  m_Impl->m_ImageSeriesTable->clearContents();
  for (auto it = seriesVector.begin(); it != seriesVector.end(); ++it)
  {
    m_Impl->m_ImageSeriesTable->addRow((*it)->GetImageInfo());
  }
}