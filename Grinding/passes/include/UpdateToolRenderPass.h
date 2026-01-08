#ifndef UPDATE_TOOL_RENDER_PASS_H
#define UPDATE_TOOL_RENDER_PASS_H
#include <memory>
#include <vtkOpenGLRenderPass.h>
#include "GrindingTool.h"
#include "PassesExport.h"

class vtkTextureObject;
class vtkMatrix4x4;
class PASSES_API UpdateToolRenderPass : public vtkOpenGLRenderPass
{
public:
  static UpdateToolRenderPass* New();
  vtkTypeMacro(UpdateToolRenderPass, vtkOpenGLRenderPass);

  void Render(const vtkRenderState* s) override;
  void ReleaseGraphicsResources(vtkWindow* w) override;
  void SetTool(Grinding::GrindingTool grindingTool, vtkTextureObject* toolTex);
  void UpdateToolMatrix(vtkMatrix4x4* matrix);
  void SetWorkpieceParams(double* origin, double* spacing, int* dimensions);
protected:
  UpdateToolRenderPass();
  ~UpdateToolRenderPass() override;

  static void GetFloatMatrix4x4(const vtkMatrix4x4* matrix, float fMatrix[16]);

private:
  struct Private;
  std::unique_ptr<Private> mPrivate;
};

#endif // UPDATE_TOOL_RENDER_PASS_H