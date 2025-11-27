#include "Window.h"
#include <QApplication>
#include <qdebug.h>
#include "Log.h"
#include <QIcon>
#include "MainWindow.h"

int main(int argc, char** argv)
{
	QApplication app(argc, argv);
	Log::initLog(app, spdlog::level::debug);

	MainWindow mainWindow;
	mainWindow.resize(1920, 1080);
	mainWindow.setWindowIcon(QIcon(":/icon/resources/icon/icon.ico"));
	mainWindow.setWindowTitle("OViewer");
	mainWindow.show();

	return app.exec();
}
