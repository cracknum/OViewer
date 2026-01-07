#ifndef OPAQUE_DEPTH_RENDER_PASS
#define OPAQUE_DEPTH_RENDER_PASS
#include <vtkOpenGLRenderPass.h>
#include <memory>
#include "PassesExport.h"

class vtkActor;
class vtkRenderer;
class vtkActorCollection;

class PASSES_API OpaqueDepthRenderPass : public vtkOpenGLRenderPass
{
public:
  static OpaqueDepthRenderPass* New();
  vtkTypeMacro(OpaqueDepthRenderPass, vtkOpenGLRenderPass);
  void Render(const vtkRenderState* s) override;
  void ReleaseGraphicsResources(vtkWindow* w) override;

  void SetOpaqueActors(vtkActorCollection* actors);

protected:
  OpaqueDepthRenderPass();
  ~OpaqueDepthRenderPass() override;

private:
  OpaqueDepthRenderPass(const OpaqueDepthRenderPass&) = delete;
  OpaqueDepthRenderPass& operator=(const OpaqueDepthRenderPass&) = delete;

  void RenderDepthTextureToColorTexture(vtkRenderer* renderer);

  struct Private;
  std::unique_ptr<Private> mPrivate;
};

#endif // OPAQUE_DEPTH_RENDER_PASS
