#include "MainWindow.h"
#include "DicomReader.hpp"
#include "ImageInformation.hpp"
#include "ProjectManagePanel.h"
#include <QFileDialog>
#include <QGridLayout>
#include <QVTKOpenGLNativeWidget.h>
#include <SARibbonBar.h>
#include <SARibbonCategory.h>
#include <itkImage.h>
#include <spdlog/spdlog.h>
#include "Window.h"
#include <vtkFloatArray.h>

namespace
{
using FloatDicomReader = DicomReadReader<itk::Image<float, 3>>;
}
struct MainWindow::Impl
{
  FloatDicomReader::SeriesVector m_SeriesVector;
  ProjectManagePanel* m_Panel;
  Window* m_Window;
  Impl()
    : m_Panel(nullptr)
    , m_Window(nullptr)
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
  SARibbonCategory* fileCategory = ribbon->addCategoryPage("File");
  fileCategory->setObjectName("File");
  auto* filePanel = fileCategory->addPanel("Files");
  filePanel->setObjectName("filePanel");
  auto* openFolderAction = new QAction(QIcon(":/icon/resources/openFolder.svg"), "打开", this);
  openFolderAction->setObjectName("open_folder");
  openFolderAction->setShortcut(QKeySequence::Open);
  filePanel->addLargeAction(openFolderAction);

  connect(openFolderAction, &QAction::triggered, this, &MainWindow::openFolder);
}

void MainWindow::initCentral()
{
  m_Impl->m_Window = new Window();
  vtkSmartPointer<vtkFloatArray> window1Setting = vtkSmartPointer<vtkFloatArray>::New();
  vtkSmartPointer<vtkFloatArray> window2Setting = vtkSmartPointer<vtkFloatArray>::New();
  vtkSmartPointer<vtkFloatArray> window3Setting = vtkSmartPointer<vtkFloatArray>::New();
  vtkSmartPointer<vtkFloatArray> window4Setting = vtkSmartPointer<vtkFloatArray>::New();
  vtkSmartPointer<vtkFloatArray> background = vtkSmartPointer<vtkFloatArray>::New();
  vtkSmartPointer<vtkFloatArray> verticalSplitLine = vtkSmartPointer<vtkFloatArray>::New();
  vtkSmartPointer<vtkFloatArray> horizontalSplitLine = vtkSmartPointer<vtkFloatArray>::New();
  vtkSmartPointer<vtkFloatArray> splitLineColor = vtkSmartPointer<vtkFloatArray>::New();

  background->SetNumberOfComponents(4);
  background->InsertNextTuple4(0.0f, 0.0f, 0.0f, 1.0f);

  verticalSplitLine->SetNumberOfComponents(3);
  verticalSplitLine->InsertNextTuple3(0.5f, 0.0f, 0.0f);
  verticalSplitLine->InsertNextTuple3(0.5f, 1.0f, 0.0f);
  horizontalSplitLine->SetNumberOfComponents(3);
  horizontalSplitLine->InsertNextTuple3(0.0f, 0.5f, 0.0f);
  horizontalSplitLine->InsertNextTuple3(1.0f, 0.5f, 0.0f);
  splitLineColor->SetNumberOfComponents(4);
  splitLineColor->InsertNextTuple4(0.0f, 1.0f, 0.0f, 1.0f);
  splitLineColor->InsertNextTuple4(0.0f, 1.0f, 0.0f, 1.0f);
  
  window1Setting->SetNumberOfComponents(4);
  window2Setting->SetNumberOfComponents(4);
  window3Setting->SetNumberOfComponents(4);
  window4Setting->SetNumberOfComponents(4);
  window1Setting->InsertNextTuple4(0.0f, 0.5f, 0.5f, 1.0f);
  window2Setting->InsertNextTuple4(0.5f, 0.5f, 1.0f, 1.0f);
  window3Setting->InsertNextTuple4(0.0f, 0.0f, 0.5f, 0.5f);
  window4Setting->InsertNextTuple4(0.5f, 0.0f, 1.0f, 0.5f);
  m_Impl->m_Window->addViewWindow(0, window1Setting, background);
  m_Impl->m_Window->addViewWindow(1, window2Setting, background);
  m_Impl->m_Window->addViewWindow(2, window3Setting, background);
  m_Impl->m_Window->addViewWindow(3, window4Setting, background);
  m_Impl->m_Window->addSplitLine(splitLineColor, horizontalSplitLine);
  m_Impl->m_Window->addSplitLine(splitLineColor, verticalSplitLine);

  auto* centerWidget = new QWidget;
  auto* layout = new QGridLayout;
  m_Impl->m_Panel = new ProjectManagePanel;
  layout->addWidget(m_Impl->m_Window, 0, 0, 1, 2);
  layout->addWidget(m_Impl->m_Panel, 0, 2, 1, 1);

  layout->setColumnStretch(0, 1);
  layout->setColumnStretch(2, 0);
  centerWidget->setLayout(layout);
  setCentralWidget(centerWidget);

  connect(m_Impl->m_Panel, &ProjectManagePanel::signalSelectedSeries, this, &MainWindow::slotOpenImage);
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

	m_Impl->m_Window->openImage(*it);
}