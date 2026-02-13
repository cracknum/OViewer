#ifndef COM_OVIEWER_DICOM_OPEN_VIEW
#define COM_OVIEWER_DICOM_OPEN_VIEW
#include "ViewInterface.h"
#include <QObject>

class DicomOpenView final
  : public QObject
  , public ViewInterface
{
  Q_OBJECT;

  Q_PLUGIN_METADATA(IID ViewInterface_iid);
  Q_INTERFACES(ViewInterface);

public:
  DicomOpenView();
  ~DicomOpenView();

  QString name() override;
  /**
   * @brief 设置插件的分类.
   */
  QString categoryName() override;
  /**
   * @brief 面板名称.
   */
  QString panelName() override;
  /**
   * @brief 动作名称.
   */
  QString actionName() override;
  /**
   * @brief 动作icon.
   */
  QIcon actionIcon() override;
  /**
   * @brief 显示widget.
   */
  QWidget* viewWidget() override;
};
#endif // COM_OVIEWER_DICOM_OPEN_VIEW
