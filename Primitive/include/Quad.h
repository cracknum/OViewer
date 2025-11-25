#ifndef PLANE_H
#define PLANE_H
#include <glm/glm.hpp>
#include "InteractiveObject.h"
#include "PrimitiveExport.h"
class PRIMITIVE_API Quad: public InteractiveObject
{
public:
  /**
   * @param origin Quad的角点
   * @param u Quad的一条边的向量
   * @param v Quad的一条边的向量
   */
  Quad(const glm::vec3& origin, const glm::vec3& u, const glm::vec3& v);
  ~Quad();

  virtual void mousePressEvent(QMouseEvent* event) override;
  virtual void mouseReleaseEvent(QMouseEvent* event) override;
  virtual void mouseDoubleClickEvent(QMouseEvent* event) override;
  virtual void mouseMoveEvent(QMouseEvent* event) override;
  virtual void wheelEvent(QWheelEvent* event) override;
  virtual void keyPressEvent(QKeyEvent* event) override;
  virtual void keyReleaseEvent(QKeyEvent* event) override;
  virtual void draw() override;

private:
  glm::vec3 m_Origin;
  glm::vec3 m_U;
  glm::vec3 m_V;
  glm::vec3 m_D;
  glm::vec3 m_Normal;
};

#endif // PLANE_H
