#ifndef PLANE_H
#define PLANE_H
#include "InteractiveObject.h"
#include "PrimitiveExport.h"
#include <glm/glm.hpp>
#include <memory>


class ShaderManager;
struct QuadConfig;
class QOpenGLFunctions_4_4_Core;

class PRIMITIVE_API Quad : public InteractiveObject
{
public:
	using Functions = QOpenGLFunctions_4_4_Core;
  /**
   * @param origin Quad的角点
   * @param u Quad的一条边的向量
   * @param v Quad的一条边的向量
   */
  Quad(Functions* functions, std::shared_ptr<ShaderManager> shaderManager, const QuadConfig& config);
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
  struct Impl;
  std::unique_ptr<Impl> m_Impl;
};

#endif // PLANE_H
