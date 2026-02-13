#include "DicomReader.hpp"
#include "ImageInformation.hpp"
#include "MainWindow.h"
#include "ProjectManagePanel.h"
#include "ViewInterface.h"
#include <itkImage.h>
#include <qapplication.h>
#include <QFileDialog>
#include <QGridLayout>
#include <qgridlayout.h>
#include <qpluginloader.h>
#include <qstackedwidget.h>
#include <qtabbar.h>
#include <SARibbonBar.h>
#include <SARibbonCategory.h>
#include <spdlog/spdlog.h>
#include <vector>
#include <vtkFloatArray.h>

namespace
{
using FloatDicomReader = DicomReadReader<itk::Image<float, 3>>;
}
struct MainWindow::Impl
{
  FloatDicomReader::SeriesVector m_SeriesVector;
  ProjectManagePanel* m_Panel;
  std::vector<std::unique_ptr<QPluginLoader>> m_ViewLoaders;
  QWidget* m_CenterWidget;
  QStackedWidget* m_CenterWidgets;
  QStackedWidget* m_PropertyWidgets;

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

  auto appPath = QApplication::applicationDirPath();
  QDir dir(appPath);
  dir.cdUp();
  dir.cdUp();
  dir.cd("Plugins");
  dir.cd("Debug");
  auto pathList = dir.entryList(QDir::Files);
  for (const auto& filePath : pathList)
  {
    if (!filePath.endsWith("dll"))
    {
      continue;
    }
    auto loader = std::make_unique<QPluginLoader>(dir.absoluteFilePath(filePath));
    QObject* plugin = loader->instance();
    if (plugin)
    {
      if (auto view = qobject_cast<ViewInterface*>(plugin))
      {
        SARibbonCategory* category = ribbon->categoryByName(view->categoryName());
        if (!category)
        {
          category = ribbon->addCategoryPage(view->categoryName());
          category->setObjectName(view->categoryName());
        }
        auto* panel = category->panelByName(view->panelName());
        if (!panel)
        {
          panel = category->addPanel(view->panelName());
          panel->setObjectName(view->panelName());
        }
		
        auto* openFolderAction = new QAction(view->actionIcon(), view->actionName(), this);
        openFolderAction->setObjectName(view->actionName());
        panel->addLargeAction(openFolderAction);

        SPDLOG_INFO("load {} plugin succeed", view->name().toStdString());
        m_Impl->m_ViewLoaders.emplace_back(std::move(loader));
      }
    }
    else
    {
      SPDLOG_ERROR("load plugin from {} failed", dir.absoluteFilePath(filePath).toStdString());
      return;
    }
  }
}

void MainWindow::initCentral()
{
  auto gridLayout = new QGridLayout();
  m_Impl->m_CenterWidgets = new QStackedWidget();
  m_Impl->m_PropertyWidgets = new QStackedWidget();
  
  gridLayout->addWidget(m_Impl->m_CenterWidgets, 0, 0, 1, 1);
  gridLayout->addWidget(m_Impl->m_PropertyWidgets, 0, 1, 1, 1);
  gridLayout->setColumnStretch(0, 3);
  gridLayout->setColumnStretch(1, 1);
  auto defaultCenterWidget = new QWidget();
  defaultCenterWidget->setObjectName("defaultCenterWidget");
  defaultCenterWidget->setStyleSheet("background-color:black;");
  auto defaultPropertyWidget = new QWidget();
  defaultPropertyWidget->setObjectName("defaultPropertyWidget");
  defaultPropertyWidget->setStyleSheet("background-color:red;");
  m_Impl->m_CenterWidgets->addWidget(defaultCenterWidget);
  m_Impl->m_PropertyWidgets->addWidget(defaultPropertyWidget);
  auto centerWidget = new QWidget();
  centerWidget->setLayout(gridLayout);
  setCentralWidget(centerWidget);
}
