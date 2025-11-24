#ifndef UI_MAIN_WINDOW_H
#define UI_MAIN_WINDOW_H

#include "UIExport.h"
#include <SARibbonMainWindow.h>
#include <memory>

class UI_API MainWindow final : public SARibbonMainWindow
{
  Q_OBJECT
public:
  explicit MainWindow(QWidget* parent = nullptr);
  ~MainWindow() override;

private:
  void initUI();
  void initHeader();
  void initCentral();
private slots:
  void openFolder();

private:
  struct Impl;
  std::unique_ptr<Impl> m_Impl;
};
#endif
