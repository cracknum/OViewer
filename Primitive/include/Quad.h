#ifndef PLANE_H
#define PLANE_H
#include "Primitive.h"
#include "PrimitiveExport.h"
#include <memory>

class ShaderManager;
struct QuadConfig;

class PRIMITIVE_API Quad final : public Primitive
{
public:
  using Superclass = Primitive;
  /**
   * @param shaderManager
   */
  explicit Quad(const std::shared_ptr<ShaderManager>& shaderManager);
  ~Quad() override;

  void setVertices(const std::shared_ptr<Vertices>& vertices) override;
  void draw() override;

private:
  struct Impl;
  std::unique_ptr<Impl> m_Impl;
};

#endif // PLANE_H
