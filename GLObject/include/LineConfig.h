#ifndef LINE_CONFIG_H
#define LINE_CONFIG_H
#include "GLObjectExport.h"
#include <QPoint>
#include <QVector4D>
struct GLOBJECT_API LineConfig
{
  using Point = QPointF;
  using Color = QVector4D;
  Point m_StartPoint;
  Point m_EndPoint;
  float m_LineWidth;
  Color m_Color;
};
#endif //LINE_CONFIG_H
