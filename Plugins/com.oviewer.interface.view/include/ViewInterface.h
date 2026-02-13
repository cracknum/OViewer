#ifndef COM_OVIEWER_INTERFACE_VIEW_H
#define COM_OVIEWER_INTERFACE_VIEW_H
#include "com_oviewer_interaface_viewExport.h"
#include <QIcon>
#include <QObject>
#include <QString>

class QWidget;
/**
 * @brief 显示UI的接口，用于在UI中显示对应的内容，所以需要显示UI的插件都应该继承ViewInteraface.
 */
class COM_OVIEWER_INTERAFACE_VIEW_API ViewInterface
{
public:
  virtual ~ViewInterface() = default;
  virtual QString name() = 0;
  /**
   * @brief 设置插件的分类.
   */
  virtual QString categoryName() = 0;
  /**
   * @brief 面板名称.
   */
  virtual QString panelName() = 0;
  /**
   * @brief 动作名称.
   */
  virtual QString actionName() = 0;
  /**
   * @brief 动作icon.
   */
  virtual QIcon actionIcon() = 0;
  /**
   * @brief 显示widget.
   */
  virtual QWidget* viewWidget() = 0;

protected:
  QWidget* m_ViewWidget = nullptr;
};

#define ViewInterface_iid "com.oviewer.interface.view"
Q_DECLARE_INTERFACE(ViewInterface, ViewInterface_iid);
#endif // COM_OVIEWER_INTERFACE_VIEW_H
