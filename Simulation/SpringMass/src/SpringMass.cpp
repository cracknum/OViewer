#include "SpringMass.h"
#include "common.hpp"
#include <vector>

namespace Simulation::SpringMass
{
struct SpringMass::Private
{
  std::vector<Particle> m_Particles;
  std::vector<Spring> m_Springs;
};

void SpringMass::SetParticles(std::vector<Particle> particles) {}

void SpringMass::predictPositions(float dt) {
	float dt2 = dt * dt;

	for (auto& p : m_Private->m_Particles)
	{
		if (p.m_InvertMass == 0)
		{
			continue;
		}
		
		p.m_PrePos = p.m_Pos;
		p.m_Pos += (p.m_Pos - p.m_PrePos) + p.m_Force * dt2;
	}
	
}

Simulation::SpringMass::SpringMass::SpringMass()
{
  m_Private = std::make_unique<Private>();
}

Simulation::SpringMass::SpringMass::~SpringMass() = default;
void SpringMass::applyGravityForce()
{
  glm::vec3 gravity(0.0f, -9.8f, 0.0f);
  for (auto& p : m_Private->m_Particles)
  {
    p.m_Force = glm::vec3(0, 0, 0);
	if (p.m_InvertMass > 0)
	{
		p.m_Force += gravity / p.m_InvertMass;
	}
  }
}


}
