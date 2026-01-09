#ifndef COMPOSIT_RENDER_PASS_H
#define COMPOSIT_RENDER_PASS_H
#include <memory>
#include <vtkOpenGLRenderPass.h>

class vtkTextureObject;
class CompositRenderPass : public vtkOpenGLRenderPass
{
public:
  static CompositRenderPass* New();
  vtkTypeMacro(CompositRenderPass, vtkOpenGLRenderPass);
  void Render(const vtkRenderState* s) override;
  void ReleaseGraphicsResources(vtkWindow* w) override;

  void SetOpaqueDepthTexture(vtkTextureObject* opaqueDepthTexture);
  void SetToolTexture(vtkTextureObject* toolTexture);
  
protected:
  CompositRenderPass();
  ~CompositRenderPass() override;

private:
  struct Private;
  std::unique_ptr<Private> mPrivate;
};

#endif // COMPOSIT_RENDER_PASS_H