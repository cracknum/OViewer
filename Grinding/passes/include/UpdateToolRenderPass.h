#ifndef UPDATE_TOOL_RENDER_PASS_H
#define UPDATE_TOOL_RENDER_PASS_H
#include <memory>
#include <vtkOpenGLRenderPass.h>
#include "GrindingTool.h"

class vtkTextureObject;
class vtkMatrix4x4;
class UpdateToolRenderPass : public vtkOpenGLRenderPass
{
public:
  static UpdateToolRenderPass* New();
  vtkTypeMacro(UpdateToolRenderPass, vtkOpenGLRenderPass);

  void Render(const vtkRenderState* s) override;
  void ReleaseGraphicsResources(vtkWindow* w) override;
  void SetTool(Grinding::GrindingTool grindingTool, vtkTextureObject* toolTex);
  void UpdateToolMatrix(vtkMatrix4x4* matrix);
protected:
  UpdateToolRenderPass();
  ~UpdateToolRenderPass() override;

private:
  struct Private;
  std::unique_ptr<Private> mPrivate;
};

#endif // UPDATE_TOOL_RENDER_PASS_H