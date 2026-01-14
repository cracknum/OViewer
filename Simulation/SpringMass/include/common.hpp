#ifndef SIMULATION_SPRING_MASS_COMMON_H
#define SIMULATION_SPRING_MASS_COMMON_H
#include <vtkVector.h>

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
  vtkVector3f pos;
  /**
   * 当前质点上一帧位置
   */
  vtkVector3f prePos;
  /**
   * 质点所受外力
   */
  vtkVector3f force;
  /**
   * 质量倒数
   */
  float inv_mass;
};

struct Spring final
{
  /**
   * 质点索引@see struct Particle
   */
  int particleIndex1;
  /**
   * 质点索引@see struct Particle
   */
  int particleIndex2;
  /**
   * 自然长度（未变形前）
   */
  float restLength;
  /**
   * 刚度，刚度越大形变速度越快
   */
  float stiffness;
  /**
   * compliance = \frac{1}{k * dt^2}
   */
  float alpha;
  /**
   * 拉格朗日乘子（每帧重置为0）
   */
  float lambda;
};

}

#endif // SIMULATION_SPRING_MASS_COMMON_H