#ifndef A_BUFFER_RENDER_PASS_H
#define A_BUFFER_RENDER_PASS_H
#include "PassesExport.h"
#include <memory>
#include <vtkOpenGLRenderPass.h>

class vtkTextureObject;
class vtkActorCollection;
class PASSES_API ABufferRenderPass : public vtkOpenGLRenderPass
{
public:
  static ABufferRenderPass* New();
  vtkTypeMacro(ABufferRenderPass, vtkOpenGLRenderPass);
  void Render(const vtkRenderState* s) override;
  void ReleaseGraphicsResources(vtkWindow* w);
  bool PreReplaceShaderValues(std::string& vertexShader, std::string& geometryShader,
    std::string& fragmentShader, vtkAbstractMapper* mapper, vtkProp* prop) override;
  bool SetShaderParameters(vtkShaderProgram* program, vtkAbstractMapper* mapper, vtkProp* prop,
    vtkOpenGLVertexArrayObject* VAO = nullptr) override;
  vtkTextureObject* GetHeadPointerImage() const;

  void SetRenderActorCollection(vtkActorCollection* collection);

protected:
  ABufferRenderPass();
  ~ABufferRenderPass() override;

private:
  ABufferRenderPass(const ABufferRenderPass&) = delete;
  ABufferRenderPass& operator=(const ABufferRenderPass&) = delete;

  struct Private;
  std::unique_ptr<Private> mPrivate;
};

#endif // A_BUFFER_RENDER_PASS_H
