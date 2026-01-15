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
#include <vtkShaderProgram.h>
#include <spdlog/spdlog.h>
#include <vtkShader.h>

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
        mPrivate->mDepthTexture->AllocateDepth(
          viewportSize[0], viewportSize[1], vtkTextureObject::Fixed32);
      }
      else
      {
        mPrivate->mDepthTexture->AllocateDepth(
          viewportSize[0], viewportSize[1], vtkTextureObject::Fixed24);
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
  // 只有执行PreRender时才能调用替换shader
  this->PreRender(s);
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

  mPrivate->mFrameBuffer->Bind(vtkOpenGLFramebufferObject::GetReadMode());
  renderWindow->GetRenderFramebuffer()->Bind(vtkOpenGLFramebufferObject::GetDrawMode());
  state->vtkglBlitFramebuffer(0, 0, viewportSize[0], viewportSize[1], 0, 0, viewportSize[0],
    viewportSize[1], GL_COLOR_BUFFER_BIT, GL_LINEAR);
  state->PopFramebufferBindings();
  this->PostRender(s);
}
/**
 *
 * PreReplaceShaderValues会在所有的mapper执行之前替换指定的shader代码，
 * 但是，即使在PreReplaceShaderValues中替换了变量也无法在PreReplaceShaderValues中设置变量，
 * 变量的设置要在重写的SetShaderParameters中完成
 *
 * 所有这一切的发生都要在Render方法中调用PreRender才能执行，否则就不会被执行
 *
 */
bool ColorChangePass::PreReplaceShaderValues(std::string& vertexShader, std::string& geometryShader,
  std::string& fragmentShader, vtkAbstractMapper* mapper, vtkProp* prop)
{
  // 否则，注入自定义代码
  bool replaced = vtkShaderProgram::Substitute(
    fragmentShader, "//VTK::CustomUniforms::Dec",
    "uniform vec3 uGlowColor;"
    "//VTK::CustomUniforms::Dec",
    false);
  SPDLOG_INFO("//VTK::Uniforms::Dec replaced:{}", replaced);

  replaced = vtkShaderProgram::Substitute(fragmentShader, "//VTK::TCoord::Impl",
    "fragOutput0.rgb += uGlowColor;"
    "//VTK::TCoord::Impl",
    false);
  SPDLOG_INFO("//VTK::TCoord::Impl, replaced: {}", replaced);
  return true;
}
bool ColorChangePass::SetShaderParameters(vtkShaderProgram* program, vtkAbstractMapper* mapper,
  vtkProp* prop, vtkOpenGLVertexArrayObject* VAO)
{
  if (program->IsUniformUsed("uGlowColor"))
  {
    constexpr float color[3] = { -0.5, 0.5, 0.5 };
    program->SetUniform3f("uGlowColor", color);
    return true;
  }
  return true;
}
void ColorChangePass::ReleaseGraphicsResources(vtkWindow* w)
{
  Superclass::ReleaseGraphicsResources(w);
}

ColorChangePass::~ColorChangePass() {}
