#ifndef PLANE_DATA_H
#define PLANE_DATA_H
#include "DataExport.h"
#include <glm/glm.hpp>

struct DATA_API Plane
{
  Plane(const glm::vec3& origin, const glm::vec3& right, const glm::vec3& bottom, const glm::mat4& indexToWorld);

  glm::vec3 m_Origin;
  glm::vec3 m_Right;
  glm::vec3 m_Bottom;
  int m_Width;
  int m_Height;

  glm::mat4 m_IndexToWorld;
  glm::mat4 m_WorldToIndex;
};
#endif // PLANE_DATA_H