#ifndef COMPOSITE_RENDER_PASS_H
#define COMPOSITE_RENDER_PASS_H
#include "PassesExport.h"
#include <memory>
#include <vtkOpenGLRenderPass.h>

class vtkTextureObject;
class PASSES_API CompositeRenderPass final : public vtkOpenGLRenderPass
{
public:
  static CompositeRenderPass* New();
  vtkTypeMacro(CompositeRenderPass, vtkOpenGLRenderPass);
  void Render(const vtkRenderState* s) override;
  void ReleaseGraphicsResources(vtkWindow* w) override;

  void SetOpaqueDepthTexture(vtkTextureObject* opaqueDepthTexture);
  void SetHeadPointerImage(vtkTextureObject* headPointerImage);
  void SetToolTexture(vtkTextureObject* toolTexture);
  bool HasOpaqueDepthTexture() const;
  bool HasHeadPointerImage() const;
  bool HasToolTexture() const;
  void SetWorkpieceParms(double* origin, double* spacing, int* dimensions);
  
protected:
  CompositeRenderPass();
  ~CompositeRenderPass() override;

private:
  struct Private;
  std::unique_ptr<Private> mPrivate;
};

#endif // COMPOSITE_RENDER_PASS_H
