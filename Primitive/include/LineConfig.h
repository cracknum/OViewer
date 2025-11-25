#ifndef LINE_CONFIG_H
#define LINE_CONFIG_H
#include "PrimitiveExport.h"
#include <QPoint>
#include <QVector4D>
#include <QVector2D>
struct PRIMITIVE_API LineConfig
{
  using Point = QPointF;
  using Color = QVector4D;
  using Size = QVector2D;
  Point m_StartPoint;
  Point m_EndPoint;
  int m_LineWidth;
  Color m_Color;
  Size m_ViewPortSize;
};
#endif //LINE_CONFIG_H
