#ifndef UI_CONTEXT_H
#define UI_CONTEXT_H
#include "GLExport.h"
#include <RenderContext.hpp>

struct ImGuiContext;

class GL_API UIContext final: public RenderContext
{
public:
    using superclass = RenderContext;
    bool init(IWindow *window) override;
    static ImGuiContext *UIContext::GetContext();
    void preRender() override;
    void postRender() override;
    void resize(int width, int height) override;
    void end() override;
};
#endif // UI_CONTEXT_H