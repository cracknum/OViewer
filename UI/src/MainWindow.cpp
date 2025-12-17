#include "MainWindow.h"
#include "DicomReader.hpp"
#include "ImageInformation.hpp"
#include "ProjectManagePanel.h"
#include <QFileDialog>
#include <QGridLayout>
#include <SARibbonBar.h>
#include <SARibbonCategory.h>
#include <itkImage.h>
#include <spdlog/spdlog.h>
#include <vtkFloatArray.h>


namespace
{
using FloatDicomReader = DicomReadReader<itk::Image<float, 3>>;
}
struct MainWindow::Impl
{
  FloatDicomReader::SeriesVector m_SeriesVector;
  ProjectManagePanel* m_Panel;
  Impl()
    : m_Panel(nullptr)
  {
  }
};

MainWindow::MainWindow(QWidget* parent /* = nullptr*/)
  : SARibbonMainWindow(parent,
      SARibbonMainWindowStyleFlag::UseRibbonMenuBar | SARibbonMainWindowStyleFlag::UseNativeFrame)
{
  m_Impl = std::make_unique<Impl>();
  initHeader();
  initCentral();
}

MainWindow::~MainWindow() = default;

void MainWindow::initUI()
{
  initHeader();
  initCentral();
}

void MainWindow::initHeader()
{
  SARibbonBar* ribbon = ribbonBar();
  SARibbonCategory* fileCategory = ribbon->addCategoryPage(tr("File"));
  fileCategory->setObjectName("File");
  auto* filePanel = fileCategory->addPanel(tr("Files"));
  filePanel->setObjectName(tr("filePanel"));
  auto* openFolderAction = new QAction(QIcon(":/icon/resources/openFolder.svg"), tr("Open"), this);
  openFolderAction->setObjectName(tr("open_folder"));
  openFolderAction->setShortcut(QKeySequence::Open);
  filePanel->addLargeAction(openFolderAction);

  connect(openFolderAction, &QAction::triggered, this, &MainWindow::openFolder);
}

void MainWindow::initCentral()
{
}

void MainWindow::openFolder()
{
  if (!m_Impl->m_SeriesVector.empty())
  {
    m_Impl->m_SeriesVector.clear();
  }

  const QString folderPath = QFileDialog::getExistingDirectory();
  SPDLOG_INFO("get folder path: {0}", folderPath.toStdString());

  if (folderPath.isEmpty())
  {
    return;
  }

  const FloatDicomReader::Pointer reader = FloatDicomReader::New();
  reader->SetDicomDirectory(folderPath.toStdString());
  reader->GenerateData();

  for (auto it = reader->begin(); it != reader->end(); ++it)
  {
    (*it)->GetImageInfo()->Print(std::cout);
    m_Impl->m_SeriesVector.push_back(*it);
  }

  m_Impl->m_Panel->slotSetImageTable(m_Impl->m_SeriesVector);
  SPDLOG_INFO("read finished");
}

void MainWindow::slotOpenImage(const QString& seriesId)
{
	auto it = std::find_if(m_Impl->m_SeriesVector.begin(), m_Impl->m_SeriesVector.end(), [&seriesId](const DicomSeries* info){
		return seriesId == QString::fromStdString(info->GetSeriesInfo()->GetNumber());
	});

	if (it == m_Impl->m_SeriesVector.end())
	{
		return;
	}
}