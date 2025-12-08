#ifndef PLANE_H
#define PLANE_H
#include "PrimitiveExport.h"
#include <glm/glm.hpp>
#include <memory>
#include "Primitive.h"

class ShaderManager;
struct QuadConfig;
class QOpenGLFunctions_4_4_Core;

class PRIMITIVE_API Quad: public Primitive
{
public:
  using Functions = QOpenGLFunctions_4_4_Core;
  using Superclass = Primitive;
  /**
   * @param origin Quad的角点
   * @param u Quad的一条边的向量
   * @param v Quad的一条边的向量
   */
  Quad(
    Functions* functions, std::shared_ptr<ShaderManager> shaderManager, const QuadConfig& config);
  ~Quad();

  void setVertices(const std::shared_ptr<Vertices>& vertices) override;
  void draw() override;

private:
  struct Impl;
  std::unique_ptr<Impl> m_Impl;
};

#endif // PLANE_H
