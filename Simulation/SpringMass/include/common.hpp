#ifndef SIMULATION_SPRING_MASS_COMMON_H
#define SIMULATION_SPRING_MASS_COMMON_H
#include <glm/glm.hpp>

namespace Simulation::SpringMass
{
/**
 * 质点数据结构
 */
struct Particle final
{
  /**
   * 当前质点位置
   */
  glm::vec3 m_Pos;
  /**
   * 当前质点上一帧位置
   */
  glm::vec3 m_PrePos;
  /**
   * 质点所受外力
   */
  glm::vec3 m_Force;
  /**
   * 质量倒数
   */
  float m_InvertMass;

  Particle(const glm::vec3 pos, float mass = 1.0f)
  :m_Pos(pos), m_PrePos(pos), m_Force(0.0f), m_InvertMass(mass > 0 ? 1.0f / mass : 0.0f) 
  {
	
  }
};

struct Spring final
{
  /**
   * 质点索引@see struct Particle
   */
  int m_ParticleIndex1;
  /**
   * 质点索引@see struct Particle
   */
  int m_ParticleIndex2;
  /**
   * 自然长度（未变形前）
   */
  float m_RestLength;
  /**
   * 刚度，刚度越大形变速度越快
   */
  float m_Stiffness;
  /**
   * compliance = \frac{1}{k * dt^2}
   */
  float m_Alpha;
  /**
   * 拉格朗日乘子（每帧重置为0）
   */
  float m_Lambda;

  Spring(int particleIndex1, int particleIndex2, const std::vector<Particle>& ps, float k)
  :m_ParticleIndex1(particleIndex1), m_ParticleIndex2(particleIndex2), m_Stiffness(k), m_Lambda(0.0)
  {
	m_RestLength = vtkMath::Distance2BetweenPoints(ps[particleIndex1].m_Pos.GetData(), ps[particleIndex2].m_Pos.GetData());
  }
};

}

#endif // SIMULATION_SPRING_MASS_COMMON_H