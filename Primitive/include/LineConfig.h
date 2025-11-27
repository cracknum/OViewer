#ifndef LINE_CONFIG_H
#define LINE_CONFIG_H
#include "PrimitiveExport.h"
#include <glm/glm.hpp>
struct PRIMITIVE_API LineConfig
{
  using Point = glm::vec2;
  using Color = glm::vec4;
  using Size = glm::vec2;
  Point m_StartPoint;
  Point m_EndPoint;
  int m_LineWidth;
  Color m_Color;
  Size m_ViewPortSize;
};
#endif //LINE_CONFIG_H
