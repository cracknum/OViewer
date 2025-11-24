#ifndef UI_MAIN_WINDOW_H
#define UI_MAIN_WINDOW_H

#include <SARibbonMainWindow.h>
#include "mainwindow.h"
#include "UIExport.h"
#include <memory>

class UI_API MainWindow final : public SARibbonMainWindow
{
	Q_OBJECT
public:
	MainWindow(QWidget* parent = nullptr);
	~MainWindow();

private:
	void initUI();
	void initHeader();
	void initCentral();
private slots:
	void openFolder();

signals:
	void signalOpenFoloderFinish(const QString& dirPath);

private:
	struct Impl;
	std::unique_ptr<Impl> m_Impl;
};
#endif 
