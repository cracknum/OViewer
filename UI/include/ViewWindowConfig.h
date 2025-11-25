#ifndef VIEW_WINDOW_CONFIG_H
#define VIEW_WINDOW_CONFIG_H
#include <QObject>
#include <QRectF>
#include <QVector4D>
#include <array>
/**
 * @brief 视口信息配置
 */
struct ViewWindowConfig
{
  using ViewPort = QRectF;
  using WindowSize = std::array<int, 4>;
  using Color = QVector4D;
  using IdType = unsigned int;
  IdType m_Id;
  /**
   * @brief <x, y, z, w> <x, y>: start point, <z, w> 相比于父窗口的宽高比例 x, y, z,
   * w都是归一化后的数据
   */
  ViewPort m_ViewPort;
  /**
   * @brief ViewPort对应的像素坐标下的值
   */
  WindowSize m_ViewPortSize;
  /**
   * @brief 背景颜色
   */
  Color m_BackgroundColor;
};

#endif