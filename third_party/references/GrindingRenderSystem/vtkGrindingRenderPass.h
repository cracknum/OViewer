#ifndef vtkGrindingRenderPass_h
#define vtkGrindingRenderPass_h

#include <vtkRenderingOpenGL2Module.h>
#include <vtkRenderPass.h>
#include <vtkSmartPointer.h>
#include <vtkOpenGLFramebufferObject.h>
#include <vtkTextureObject.h>

class VTKRENDERINGOPENGL2_EXPORT vtkGrindingRenderPass : public vtkRenderPass
{
public:
  static vtkGrindingRenderPass* New();
  vtkTypeMacro(vtkGrindingRenderPass, vtkRenderPass);
  void PrintSelf(ostream& os, vtkIndent indent) override;

  void SetSceneColorTexture(vtkTextureObject* colorTex);
  void SetSceneDepthTexture(vtkTextureObject* depthTex);
  void SetRemovalVolume(vtkTextureObject* volume);

  vtkSetVector3xMacro(VolumeMin, double);
  vtkGetVector3xMacro(VolumeMin, double);
  vtkSetVector3xMacro(VolumeMax, double);
  vtkGetVector3xMacro(VolumeMax, double);

  void Render(const vtkRenderState* s) override;

protected:
  vtkGrindingRenderPass();
  ~vtkGrindingRenderPass() override;

  void PrepareFullScreenQuad(vtkOpenGLRenderWindow* renWin);
  void RenderFullScreenQuad(vtkOpenGLRenderWindow* renWin, const vtkRenderState* s);

  vtkSmartPointer<vtkTextureObject> SceneColorTexture;
  vtkSmartPointer<vtkTextureObject> SceneDepthTexture;
  vtkSmartPointer<vtkTextureObject> RemovalVolume;

  double VolumeMin[3] = {-1.0, -1.0, -1.0};
  double VolumeMax[3] = { 1.0,  1.0,  1.0};

  unsigned int FullScreenVAO = 0;
  unsigned int FullScreenVBO = 0;
  bool QuadPrepared = false;

private:
  vtkGrindingRenderPass(const vtkGrindingRenderPass&) = delete;
  void operator=(const vtkGrindingRenderPass&) = delete;
};

#endif