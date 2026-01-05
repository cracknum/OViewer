#include "ColorChangePass.h"

#include <vtkImageData.h>
#include <vtkObjectFactory.h>
#include <vtkOpenGLError.h>
#include <vtkOpenGLFramebufferObject.h>
#include <vtkOpenGLRenderWindow.h>
#include <vtkOpenGLRenderer.h>
#include <vtkOpenGLState.h>
#include <vtkRenderState.h>
#include <vtkTextureObject.h>
#include <vtkPNGWriter.h>
#include <vtkUnsignedCharArray.h>

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
      mPrivate->mColorTexture->SetContext(renderWindow);
      mPrivate->mColorTexture->Allocate2D(viewportSize[0], viewportSize[1], 4, VTK_UNSIGNED_CHAR, 0);
      mPrivate->mColorTexture->SetContext(renderWindow);
  }
  else
  {
    mPrivate->mColorTexture->Resize(viewportSize[0], viewportSize[1]);
  }

  if (!mPrivate->mDepthTexture)
  {
    mPrivate->mDepthTexture = vtkSmartPointer<vtkTextureObject>::New();
    mPrivate->mDepthTexture->SetContext(renderWindow);
    if (renderWindow->GetStencilCapable())
    {
      mPrivate->mDepthTexture->AllocateDepthStencil(viewportSize[0], viewportSize[1]);
    }
    else
    {
      auto dbit = renderWindow->GetDepthBufferSize();
      if (dbit == 32)
      {
        mPrivate->mDepthTexture->AllocateDepth(viewportSize[0], viewportSize[1], vtkTextureObject::Fixed32);
      }
      else
      {
        mPrivate->mDepthTexture->AllocateDepth(viewportSize[0], viewportSize[1], vtkTextureObject::Fixed24);
      }
    }
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
  // 清空 FBO（否则可能看到垃圾）
  state->vtkglViewport(0, 0, viewportSize[0], viewportSize[1]);

  state->vtkglClearColor(0.0f, 0.0f, 0.0f, 1.0f); // 黑色背景
  state->vtkglClearDepth(1.0);
  state->vtkglClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  vtkOpenGLState::ScopedglEnableDisable depthSaver(state, GL_DEPTH_TEST);
  state->vtkglEnable(GL_DEPTH_TEST);
  this->UpdateCamera(renderer);
  this->UpdateLightGeometry(renderer);
  this->UpdateLights(renderer);
  this->UpdateGeometry(renderer);

  // 调整绘制的非背景图像的颜色

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
