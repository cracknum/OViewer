#ifndef COMPOSITE_RENDER_PASS_H
#define COMPOSITE_RENDER_PASS_H
#include <memory>
#include <vtkOpenGLRenderPass.h>

class vtkTextureObject;
class CompositeRenderPass final : public vtkOpenGLRenderPass
{
public:
  static CompositeRenderPass* New();
  vtkTypeMacro(CompositeRenderPass, vtkOpenGLRenderPass);
  void Render(const vtkRenderState* s) override;
  void ReleaseGraphicsResources(vtkWindow* w) override;

  void SetOpaqueDepthTexture(vtkTextureObject* opaqueDepthTexture);
  void SetToolTexture(vtkTextureObject* toolTexture);
  
protected:
  CompositeRenderPass();
  ~CompositeRenderPass() override;

private:
  struct Private;
  std::unique_ptr<Private> mPrivate;
};

#endif // COMPOSITE_RENDER_PASS_H