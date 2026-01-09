#include "CompositRenderPass.h"
#include <vtkObjectFactory.h>
#include <vtkOpenGLError.h>
#include <vtkOpenGLFramebufferObject.h>
#include <vtkOpenGLRenderWindow.h>
#include <vtkOpenGLRenderer.h>
#include <vtkOpenGLState.h>
#include <vtkRenderState.h>
#include <vtkShaderProgram.h>
#include <vtkSmartPointer.h>
#include <vtkTextureObject.h>
#include <vtkOpenGLQuadHelper.h>


struct CompositRenderPass::Private
{
  vtkSmartPointer<vtkTextureObject> mOpaqueDepthTexture;
  vtkSmartPointer<vtkTextureObject> mToolTexture;
  vtkSmartPointer<vtkOpenGLFramebufferObject> mFrameBuffer;
  vtkSmartPointer<vtkTextureObject> mColorTexture;
  std::unique_ptr<vtkOpenGLQuadHelper> mDrawHelper;
};

vtkStandardNewMacro(CompositRenderPass);

void CompositRenderPass::Render(const vtkRenderState* s)
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
  else if (mPrivate->mColorTexture->GetWidth() != windowSize[0] || mPrivate->mColorTexture->GetHeight() != windowSize[1])
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
	mPrivate->mDrawHelper = std::make_unique<vtkOpenGLQuadHelper>();
  }
  
  
}

void CompositRenderPass::ReleaseGraphicsResources(vtkWindow* w)
{
  Superclass::ReleaseGraphicsResources(w);
}

void CompositRenderPass::SetOpaqueDepthTexture(vtkTextureObject* opaqueDepthTexture)
{
  mPrivate->mOpaqueDepthTexture = opaqueDepthTexture;
}

void CompositRenderPass::SetToolTexture(vtkTextureObject* toolTexture)
{
  mPrivate->mToolTexture = toolTexture;
}

CompositRenderPass::CompositRenderPass()
{
  mPrivate = std::make_unique<Private>();
}

CompositRenderPass::~CompositRenderPass() = default;
