#ifndef PROPERTY_PANEL_H
#define PROPERTY_PANEL_H
#include <memory>
#include "WidgetsExport.h"
#include "OpenGLViewerWidget.h"
#include "IEventObserver.h"

struct ImGuiContext;

class WIDGETS_API SceneView: public IEventObserver, public UIObject
{
public:
    explicit SceneView(ImGuiContext* context);
    ~SceneView();

    void render(SceneView *mScene = nullptr) const;

    SceneView(const SceneView &) = delete;
    SceneView &operator=(const SceneView &) = delete;

    SceneView(SceneView &&) noexcept ;
    SceneView &operator=(SceneView &&) noexcept;
    std::shared_ptr<OpenGLViewerWidget> viewerWidget();
    bool handle(const EventObject& event) override;
private:
    struct Impl;
    std::unique_ptr<Impl> mImpl;
};

#endif