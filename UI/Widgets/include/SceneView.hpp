#ifndef PROPERTY_PANEL_H
#define PROPERTY_PANEL_H
#include "IEventObserver.h"
#include "OpenGLViewerWidget.h"
#include "WidgetsExport.h"
#include <memory>

struct ImGuiContext;

class WIDGETS_API SceneView final
  : public IEventObserver
  , public UIObject
{
public:
  explicit SceneView(ImGuiContext* context);
  ~SceneView() override;

  void render(SceneView* mScene = nullptr) const;

  SceneView(const SceneView&) = delete;
  SceneView& operator=(const SceneView&) = delete;

  SceneView(SceneView&&) noexcept;
  SceneView& operator=(SceneView&&) noexcept;
  std::shared_ptr<OpenGLViewerWidget> viewerWidget();
  bool handle(const EventObject& event) override;

private:
  struct Impl;
  std::unique_ptr<Impl> mImpl;
};

#endif