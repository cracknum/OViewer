#include "CompositeRenderPass.h"
#include <vtkObjectFactory.h>
#include <vtkOpenGLError.h>
#include <vtkOpenGLFramebufferObject.h>
#include <vtkOpenGLQuadHelper.h>
#include <vtkOpenGLRenderWindow.h>
#include <vtkOpenGLRenderer.h>
#include <vtkOpenGLState.h>
#include <vtkRenderState.h>
#include <vtkShaderProgram.h>
#include <vtkSmartPointer.h>
#include <vtkTextureObject.h>

struct CompositeRenderPass::Private
{
  vtkSmartPointer<vtkTextureObject> mOpaqueDepthTexture;
  vtkSmartPointer<vtkTextureObject> mToolTexture;
  vtkSmartPointer<vtkOpenGLFramebufferObject> mFrameBuffer;
  vtkSmartPointer<vtkTextureObject> mColorTexture;
  std::unique_ptr<vtkOpenGLQuadHelper> mDrawHelper;
};

vtkStandardNewMacro(CompositeRenderPass);

void CompositeRenderPass::Render(const vtkRenderState* s)
{
  auto renderer = vtkOpenGLRenderer::SafeDownCast(s->GetRenderer());
  auto renderWindow = vtkOpenGLRenderWindow::SafeDownCast(renderer->GetRenderWindow());
  auto ostate = renderWindow->GetState();
  int* windowSize = renderWindow->GetSize();

  if (!mPrivate->mColorTexture)
  {
    mPrivate->mColorTexture = vtkSmartPointer<vtkTextureObject>::New();
    mPrivate->mColorTexture->SetContext(renderWindow);
    mPrivate->mColorTexture->Allocate2D(windowSize[0], windowSize[1], 4, VTK_UNSIGNED_CHAR, 0);
  }
  else if (mPrivate->mColorTexture->GetWidth() != windowSize[0] ||
    mPrivate->mColorTexture->GetHeight() != windowSize[1])
  {
    mPrivate->mColorTexture->Resize(windowSize[0], windowSize[1]);
  }

  if (!mPrivate->mFrameBuffer)
  {
    mPrivate->mFrameBuffer = vtkSmartPointer<vtkOpenGLFramebufferObject>::New();
    mPrivate->mFrameBuffer->SetContext(renderWindow);
    ostate->PushFramebufferBindings();
    mPrivate->mFrameBuffer->Bind();
    mPrivate->mFrameBuffer->AddColorAttachment(0, mPrivate->mColorTexture);
    ostate->PopFramebufferBindings();
  }

  if (!mPrivate->mDrawHelper)
  {
    this->UpdateLights(renderer);
    this->UpdateLightGeometry(renderer);
    this->UpdateCamera(renderer);
    // mPrivate->mDrawHelper = std::make_unique<vtkOpenGLQuadHelper>();
  }
}

void CompositeRenderPass::ReleaseGraphicsResources(vtkWindow* w)
{
  Superclass::ReleaseGraphicsResources(w);
}

void CompositeRenderPass::SetOpaqueDepthTexture(vtkTextureObject* opaqueDepthTexture)
{
  mPrivate->mOpaqueDepthTexture = opaqueDepthTexture;
}

void CompositeRenderPass::SetToolTexture(vtkTextureObject* toolTexture)
{
  mPrivate->mToolTexture = toolTexture;
}

CompositeRenderPass::CompositeRenderPass()
{
  mPrivate = std::make_unique<Private>();
}

CompositeRenderPass::~CompositeRenderPass() = default;
