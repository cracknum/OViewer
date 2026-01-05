#include "ColorChangePass.h"
#include <vtkObjectFactory.h>
#include <vtkOpenGLError.h>
#include <vtkOpenGLFramebufferObject.h>
#include <vtkOpenGLRenderWindow.h>
#include <vtkOpenGLRenderer.h>
#include <vtkRenderState.h>
#include <vtkTextureObject.h>
#include <vtkOpenGLState.h>

struct ColorChangePass::Private
{
  vtkSmartPointer<vtkTextureObject> mColorTexture;
  vtkSmartPointer<vtkTextureObject> mDepthTexture;
  vtkSmartPointer<vtkOpenGLFramebufferObject> mFrameBuffer;
};

vtkStandardNewMacro(ColorChangePass)

  ColorChangePass::ColorChangePass()
{
  mPrivate = std::make_unique<Private>();
}

void ColorChangePass::Render(const vtkRenderState* s)
{
  vtkOpenGLClearErrorMacro();
  auto renderer = vtkOpenGLRenderer::SafeDownCast(s->GetRenderer());
  auto renderWindow = vtkOpenGLRenderWindow::SafeDownCast(renderer->GetRenderWindow());
  renderWindow->MakeCurrent();
  auto* state = renderWindow->GetState();

  int viewportSize[2]{};
  s->GetWindowSize(viewportSize);

  if (!mPrivate->mColorTexture)
  {
    mPrivate->mColorTexture = vtkSmartPointer<vtkTextureObject>::New();
    mPrivate->mColorTexture->SetInternalFormat(GL_RGBA8);
    mPrivate->mColorTexture->SetFormat(GL_RGBA);
    mPrivate->mColorTexture->SetDataType(GL_UNSIGNED_BYTE);
    mPrivate->mColorTexture->SetContext(renderWindow);
    mPrivate->mColorTexture->Allocate2D(viewportSize[0], viewportSize[1], 4, GL_UNSIGNED_BYTE, 0);
    mPrivate->mColorTexture->SetContext(renderWindow);
  }
  else
  {
    mPrivate->mColorTexture->Resize(viewportSize[0], viewportSize[1]);
  }

  if (!mPrivate->mDepthTexture)
  {
    mPrivate->mDepthTexture = vtkSmartPointer<vtkTextureObject>::New();
    if (renderWindow->GetStencilCapable())
    {
      mPrivate->mDepthTexture->AllocateDepthStencil(viewportSize[0], viewportSize[1]);
    }
    mPrivate->mDepthTexture->SetContext(renderWindow);
  }
  else
  {
    mPrivate->mDepthTexture->Resize(viewportSize[0], viewportSize[1]);
  }

  if (!mPrivate->mFrameBuffer)
  {
    mPrivate->mFrameBuffer = vtkSmartPointer<vtkOpenGLFramebufferObject>::New();
    mPrivate->mFrameBuffer->SetContext(renderWindow);
	state->PushFramebufferBindings();
    mPrivate->mFrameBuffer->Bind();
	mPrivate->mFrameBuffer->AddDepthAttachment(mPrivate->mDepthTexture);
	mPrivate->mFrameBuffer->AddColorAttachment(0, mPrivate->mColorTexture);
    state->PopFramebufferBindings();
  }
  state->PushFramebufferBindings();
  mPrivate->mFrameBuffer->Bind(vtkOpenGLFramebufferObject::GetDrawMode());
  mPrivate->mFrameBuffer->ActivateDrawBuffer(0);
  this->UpdateCamera(renderer);
  this->UpdateLightGeometry(renderer);
  this->UpdateLights(renderer);
  this->UpdateGeometry(renderer, mPrivate->mFrameBuffer);

  mPrivate->mFrameBuffer->Bind(vtkOpenGLFramebufferObject::GetReadMode());
  renderWindow->GetRenderFramebuffer()->Bind(vtkOpenGLFramebufferObject::GetDrawMode());
  state->vtkglBlitFramebuffer(
	0, 0, viewportSize[0], viewportSize[1],
	0, 0, viewportSize[0], viewportSize[1],
	GL_COLOR_BUFFER_BIT,
	GL_LINEAR
  );
  state->PopFramebufferBindings();
}
void ColorChangePass::ReleaseGraphicsResources(vtkWindow* w)
{
  Superclass::ReleaseGraphicsResources(w);
}

ColorChangePass::~ColorChangePass() {}
