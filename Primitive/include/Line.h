#ifndef SPLIT_LINE_H
#define SPLIT_LINE_H
#include "InteractiveObject.h"
#include "LineConfig.h"
#include "PrimitiveExport.h"
#include <memory>

class QOpenGLFunctions_4_4_Core;
class ShaderManager;
class PRIMITIVE_API Line final : public InteractiveObject
{
public:
  using Functions = QOpenGLFunctions_4_4_Core;
  Line(Functions* functions, std::shared_ptr<ShaderManager> shaderManager,
    const LineConfig& lineConfig);
  ~Line();
  void mousePressEvent(QMouseEvent* event) override;
  void mouseReleaseEvent(QMouseEvent* event) override;
  void mouseDoubleClickEvent(QMouseEvent* event) override;
  void mouseMoveEvent(QMouseEvent* event) override;
  void wheelEvent(QWheelEvent* event) override;
  void keyPressEvent(QKeyEvent* event) override;
  void keyReleaseEvent(QKeyEvent* event) override;

  void draw(const glm::mat4& viewMatrix, const glm::mat4& projectMatrix) override;

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
