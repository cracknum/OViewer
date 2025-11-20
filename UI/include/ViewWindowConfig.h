#ifndef VIEW_WINDOW_CONFIG_H
#define VIEW_WINDOW_CONFIG_H
#include <QObject>
#include <QRectF>
/**
 * @brief 视口信息配置
 */
struct ViewWindowConfig
{
  /**
   * @brief 视口尺寸和位置信息
   */
  QRect m_ViewPort;
};

#endif