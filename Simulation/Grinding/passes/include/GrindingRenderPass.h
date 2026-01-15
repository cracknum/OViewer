#ifndef GRINDING_RENDER_PASS_H
#define GRINDING_RENDER_PASS_H
#include <memory>
#include <vtkOpenGLRenderPass.h>
#include "GrindingTool.h"
#include "PassesExport.h"

class vtkActorCollection;
class vtkInformationDoubleVectorKey;
class vtkInformationIntegerVectorKey;
class PASSES_API GrindingRenderPass : public vtkOpenGLRenderPass
{
public:
  static GrindingRenderPass* New();
  vtkTypeMacro(GrindingRenderPass, vtkOpenGLRenderPass);
  void Render(const vtkRenderState* s) override;
  void ReleaseGraphicsResources(vtkWindow* w) override;

  void SetWorkpieceActors(vtkActorCollection* workpieces);
  void SetTool(Grinding::GrindingTool tool);

  static vtkInformationDoubleVectorKey* SpacingInfo();
  static vtkInformationIntegerVectorKey* DimensionsInfo();
  static vtkInformationDoubleVectorKey* OriginInfo();

protected:
  GrindingRenderPass();
  ~GrindingRenderPass() override;

private:
  struct Private;
  std::unique_ptr<Private> mPrivate;
};

#endif // GRINDING_RENDER_PASS_H