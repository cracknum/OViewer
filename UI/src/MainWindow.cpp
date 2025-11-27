#include "MainWindow.h"
#include "DicomReader.hpp"
#include "ImageInformation.hpp"
#include "ProjectManagePanel.h"
#include "Quad.h"
#include "QuadConfig.h"
#include "ViewWindowBase.h"
#include "ViewWindowConfig.h"
#include "Window.h"
#include <QFileDialog>
#include <QGridLayout>
#include <SARibbonBar.h>
#include <SARibbonCategory.h>
#include <itkImage.h>
#include <spdlog/spdlog.h>


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
  auto* window = new Window();
  ViewWindowConfig config1;
  config1.m_ViewPort = ViewWindowConfig::ViewPort(0, 0, 0.5, 0.5);
  config1.m_BackgroundColor = ViewWindowConfig::Color(1.0f, .0f, .0f, 1.0f);
  config1.m_Id = 0;

  ViewWindowConfig config2;
  config2.m_ViewPort = ViewWindowConfig::ViewPort(0.5, 0, 0.5, 0.5);
  config2.m_BackgroundColor = ViewWindowConfig::Color(0.0f, 0.0f, 1.0f, 1.0f);
  config2.m_Id = 1;

  ViewWindowConfig config3;
  config3.m_ViewPort = ViewWindowConfig::ViewPort(0, 0.5, 0.5, 0.5);
  config3.m_BackgroundColor = ViewWindowConfig::Color(1.0f, 0.0f, .0f, 1.0f);
  config3.m_Id = 2;

  ViewWindowConfig config4;
  config4.m_ViewPort = ViewWindowConfig::ViewPort(0.5, 0.5, 0.5, 0.5);
  config4.m_BackgroundColor = ViewWindowConfig::Color(0.0f, 0.0f, 1.0f, 1.0f);
  config4.m_Id = 4;

  window->addViewWindow(config1);
  window->addViewWindow(config2);
  window->addViewWindow(config3);
  window->addViewWindow(config4);
  m_Impl->m_Window = window;

  auto* centerWidget = new QWidget;
  auto* layout = new QGridLayout;
  m_Impl->m_Panel = new ProjectManagePanel;
  layout->addWidget(window, 0, 0, 1, 2);
  layout->addWidget(m_Impl->m_Panel, 0, 2, 1, 1);

  layout->setColumnStretch(0, 1);
  layout->setColumnStretch(2, 0);
  centerWidget->setLayout(layout);
  setCentralWidget(centerWidget);
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
