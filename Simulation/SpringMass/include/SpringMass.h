#ifndef SIMULATION_SPRING_MASS_SPRING_MASS_H
#define SIMULATION_SPRING_MASS_SPRING_MASS_H
#include <memory>

namespace Simulation::SpringMass
{
struct Particle;
class SpringMass final
{
public:
  void SetParticles(std::vector<Particle> particles);
  void predictPositions(float dt);
protected:
  SpringMass();
  ~SpringMass();

  void applyGravityForce();

private:
  struct Private;
  std::unique_ptr<Private> m_Private;
};
}

#endif // SIMULATION_SPRING_MASS_SPRING_MASS_H