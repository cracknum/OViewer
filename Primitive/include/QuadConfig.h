#ifndef QUAD_CONFIG_H
#define QUAD_CONFIG_H
#include <glm/glm.hpp>
struct QuadConfig
{
	glm::vec3 m_Origin;
	glm::vec3 m_U;
	glm::vec3 m_V;
	// m_Normal = normalize(cross(m_U, m_V))
	glm::vec3 m_Normal;
	// m_D = dot(m_Origin, m_Normal)
	float m_D;
};
#endif // QUAD_CONFIG_H