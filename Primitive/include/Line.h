#ifndef SPLIT_LINE_H
#define SPLIT_LINE_H
#include "LineConfig.h"
#include "Primitive.h"
#include "PrimitiveExport.h"
#include <memory>

class ShaderManager;
class PRIMITIVE_API Line final : public Primitive
{
public:
  Line(const std::shared_ptr<ShaderManager>& shaderManager, const LineConfig& lineConfig);
  ~Line() override;

  void draw() override;

  LineConfig& getLineConfig();
  void setLineConfig(const LineConfig& config);
  void setViewSize(const LineConfig::Size& size);

protected:
  void setColor(const LineConfig& config);
  void setWidth(const LineConfig& config);

private:
  struct Impl;
  std::unique_ptr<Impl> m_Impl;
};

#endif // SPLIT_LINE_H
