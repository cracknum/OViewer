#include "MainWindow.h"
#include <SARibbonCategory.h>
#include <SARibbonBar.h>
#include <QFileDialog>
#include <spdlog/spdlog.h>

MainWindow::MainWindow(QWidget* parent/* = nullptr*/)
	: SARibbonMainWindow(parent,
		SARibbonMainWindowStyleFlag::UseRibbonMenuBar | SARibbonMainWindowStyleFlag::UseNativeFrame)
{
	SARibbonBar* ribbon = ribbonBar();
	SARibbonCategory* fileCategory = ribbon->addCategoryPage("File");
	fileCategory->setObjectName("File");
	auto* filePanel = fileCategory->addPanel("Files");
	filePanel->setObjectName("filePanel");
	QAction* openFolderAction = new QAction(QIcon(":/icon/resources/openFolder.svg"), "打开", this);
	openFolderAction->setObjectName("open_folder");
	openFolderAction->setShortcut(QKeySequence::Open);
	filePanel->addLargeAction(openFolderAction);

	connect(openFolderAction, &QAction::triggered, this, &MainWindow::openFolder);
}

void MainWindow::openFolder()
{
	QString folderPath = QFileDialog::getExistingDirectory();
	spdlog::info("get folder path: {0}", folderPath.toStdString());
	emit signalOpenFoloderFinish(folderPath);
}
