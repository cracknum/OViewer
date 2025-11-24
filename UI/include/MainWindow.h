#ifndef UI_MAIN_WINDOW_H
#define UI_MAIN_WINDOW_H

#include <SARibbonMainWindow.h>
#include "mainwindow.h"
#include "UIExport.h"

class UI_API MainWindow : public SARibbonMainWindow
{
	Q_OBJECT
public:
	MainWindow(QWidget* parent = nullptr);

private slots:
	void openFolder();

signals:
	void signalOpenFoloderFinish(const QString& dirPath);
};
#endif 
