#include "MainWindow.h"
#include <SARibbonCategory.h>
#include <SARibbonBar.h>

MainWindow::MainWindow(QWidget* parent/* = nullptr*/)
  : SARibbonMainWindow(parent,
      SARibbonMainWindowStyleFlag::UseRibbonMenuBar | SARibbonMainWindowStyleFlag::UseNativeFrame)
{
  // 获取 RibbonBar 指针
  SARibbonBar* ribbon = ribbonBar();
  // 直接添加一个名为“Main”的分类页
  SARibbonCategory* homeCategory = ribbon->addCategoryPage("Main");
  // 如果你需要进行ribbon界面自定义配置工具（SARibbonCustomizeDialog），你必须设置
  // objectName用于区分
  homeCategory->setObjectName("MainCategory");
}
