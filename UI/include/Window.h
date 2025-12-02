#ifndef WINDOW_H
#define WINDOW_H
#include "Macros.h"
#include "UIExport.h"
#include <QOpenGLFunctions_4_4_Core>
#include <QOpenGLWidget>
#include <QWidget>
#include <memory>
#include "ViewWindowConfig.h"
#include <glm/glm.hpp>

class ViewWindowBase;
class ViewWindow;
class ShaderManager;
class DicomSeries;
class Line;

class UI_API Window final
  : public QOpenGLWidget
  , public QOpenGLFunctions_4_4_Core
{
  Q_OBJECT
public:
  explicit Window(QWidget* parent = nullptr);

  ~Window() override;
  DISALLOW_COPY_AND_MOVE(Window);

  using ViewWindowVector = std::vector<ViewWindowBase*>;
  using ShaderManagerPointer = std::shared_ptr<ShaderManager>;

  void addViewWindow(const ViewWindowConfig& config);
  [[nodiscard]] ViewWindowBase* getViewWindow(ViewWindowConfig::IdType id) const;
  [[nodiscard]] ShaderManagerPointer getShaderManager() const;

protected:
  void initializeGL() override;
  void resizeGL(int w, int h) override;
  void paintGL() override;
  void mousePressEvent(QMouseEvent* event) override;
  void mouseReleaseEvent(QMouseEvent* event) override;
  void mouseDoubleClickEvent(QMouseEvent* event) override;
  void mouseMoveEvent(QMouseEvent* event) override;
  void wheelEvent(QWheelEvent* event) override;
  void keyPressEvent(QKeyEvent* event) override;
  void keyReleaseEvent(QKeyEvent* event) override;

  void addViewWindow(int viewId, const glm::vec4& windowRange, const glm::vec4& background);
  void addSplitLine(const glm::vec4& colorArray, const glm::vec4& linePoints);
  [[nodiscard]] ViewWindow* getViewWindow(int viewId) const;

  public slots:
  void openImage(DicomSeries* dicom);

protected:
  Line createLineActor(const glm::vec4& colorArray, const glm::vec3& lineStart, const glm::vec3& lineEnd);

private:
  struct Impl;
  std::unique_ptr<Impl> m_Impl;
};

#endif // WINDOW_H
