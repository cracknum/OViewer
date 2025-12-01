#include "Plane.h"

Plane::Plane(const glm::vec3& origin, const glm::vec3& right, const glm::vec3& bottom,
  const glm::mat4& indexToWorld)
  : m_Origin(origin)
  , m_Right(right)
  , m_Bottom(bottom)
  , m_IndexToWorld(indexToWorld)
  , m_WorldToIndex(glm::inverse(m_IndexToWorld))
{
	m_Width = glm::length(origin + right);
	m_Height = glm::length(origin + bottom);
}