#ifndef SPLIT_LINE_H
#define SPLIT_LINE_H
#include "GLObjectExport.h"
#include "InteractiveObject.h"
#include <memory>

struct LineConfig;
class QOpenGLFunctions_4_4_Core;
class GLOBJECT_API SplitLine final : public InteractiveObject
{
public:
  using Functions = QOpenGLFunctions_4_4_Core;
  SplitLine(Functions* functions, const LineConfig& lineConfig);
  ~SplitLine();
  void mousePressEvent(QMouseEvent* event) override;
  void mouseReleaseEvent(QMouseEvent* event) override;
  void mouseDoubleClickEvent(QMouseEvent* event) override;
  void mouseMoveEvent(QMouseEvent* event) override;
  void wheelEvent(QWheelEvent* event) override;
  void keyPressEvent(QKeyEvent* event) override;
  void keyReleaseEvent(QKeyEvent* event) override;

  void draw() override;

protected:
  void setColor(const LineConfig& config);
  void setWidth(const LineConfig& config);

private:
  struct Impl;
  std::unique_ptr<Impl> m_Impl;
};

#endif // SPLIT_LINE_H
