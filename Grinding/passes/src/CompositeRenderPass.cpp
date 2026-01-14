#include "CompositeRenderPass.h"
#include "preprocess.h"
#include <vtkObjectFactory.h>
#include <vtkOpenGLError.h>
#include <vtkOpenGLFramebufferObject.h>
#include <vtkOpenGLQuadHelper.h>
#include <vtkOpenGLQuadHelper.h>
#include <vtkOpenGLRenderer.h>
#include <vtkOpenGLRenderWindow.h>
#include <vtkOpenGLState.h>
#include <vtkRenderState.h>
#include <vtkShaderProgram.h>
#include <vtkSmartPointer.h>
#include <vtkTextureObject.h>

#include <spdlog/spdlog.h>
#include <vtkOpenGLShaderCache.h>

namespace
{
void PrintShaderLog(const unsigned int shader)
{
  int len = 0;
  int chWrittn = 0;
  glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
  if (len > 0)
  {
    auto log = static_cast<char*>(malloc(len));
    glGetShaderInfoLog(shader, len, &chWrittn, log);
    SPDLOG_ERROR(log);
    free(log);
  }
}
void PrintProgramLog(const unsigned int program)
{
  int len = 0;
  int chWrittn = 0;
  glGetProgramiv(program, GL_INFO_LOG_LENGTH, &len);
  if (len > 0)
  {
    auto log = static_cast<char*>(malloc(len));
    glGetProgramInfoLog(program, len, &chWrittn, log);
    SPDLOG_ERROR(log);
    free(log);
  }
}
}

struct CompositeRenderPass::Private
{
  // outside inject
  vtkSmartPointer<vtkTextureObject> mOpaqueDepthTexture;
  // outside inject
  vtkSmartPointer<vtkTextureObject> mToolTexture;
  vtkSmartPointer<vtkOpenGLFramebufferObject> mFrameBuffer;
  vtkSmartPointer<vtkTextureObject> mColorTexture;
  vtkSmartPointer<vtkTextureObject> mDepthTexture;
  // outside inject
  vtkSmartPointer<vtkTextureObject> mHeadPointerImageTexture;
  std::unique_ptr<vtkOpenGLQuadHelper> mDrawHelper;

  double mWorkpieceOrigin[3];
  double mWorkpieceSpacing[3];
  float mWorkpieceDimensions[3];

  Private()
    : mWorkpieceOrigin{}
    , mWorkpieceSpacing{}
    , mWorkpieceDimensions{}
  {
  }
};

vtkStandardNewMacro(CompositeRenderPass);

void CompositeRenderPass::Render(const vtkRenderState* s)
{
  vtkOpenGLClearErrorMacro();

  auto renderer = vtkOpenGLRenderer::SafeDownCast(s->GetRenderer());
  auto renderWindow = vtkOpenGLRenderWindow::SafeDownCast(renderer->GetRenderWindow());
  auto ostate = renderWindow->GetState();
  int* windowSize = renderWindow->GetSize();

  if (!mPrivate->mToolTexture || !mPrivate->mOpaqueDepthTexture)
  {
    SPDLOG_ERROR("param set is not completed: tool texture: {}, opaque depth texture: {}, head "
                 "pointer image texture: {}",
      mPrivate->mToolTexture != nullptr, mPrivate->mOpaqueDepthTexture != nullptr,
      mPrivate->mHeadPointerImageTexture != nullptr);
    return;
  }

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

  if (!mPrivate->mDepthTexture)
  {
    mPrivate->mDepthTexture = vtkSmartPointer<vtkTextureObject>::New();
    mPrivate->mDepthTexture->SetContext(renderWindow);
    mPrivate->mDepthTexture->SetWrapS(vtkTextureObject::Nearest);
    mPrivate->mDepthTexture->SetWrapT(vtkTextureObject::Nearest);
    mPrivate->mDepthTexture->SetMinificationFilter(vtkTextureObject::ClampToEdge);
    mPrivate->mDepthTexture->SetMagnificationFilter(vtkTextureObject::ClampToEdge);
    mPrivate->mDepthTexture->AllocateDepth(windowSize[0], windowSize[1], vtkTextureObject::Fixed24);
  }
  else if (mPrivate->mDepthTexture->GetWidth() != windowSize[0] ||
    mPrivate->mDepthTexture->GetHeight() != windowSize[1])
  {
    mPrivate->mDepthTexture->Resize(windowSize[0], windowSize[1]);
  }

  if (!mPrivate->mFrameBuffer)
  {
    mPrivate->mFrameBuffer = vtkSmartPointer<vtkOpenGLFramebufferObject>::New();
    mPrivate->mFrameBuffer->SetContext(renderWindow);
    ostate->PushFramebufferBindings();
    mPrivate->mFrameBuffer->Bind();
    mPrivate->mFrameBuffer->AddColorAttachment(0, mPrivate->mColorTexture);
    mPrivate->mFrameBuffer->AddDepthAttachment(mPrivate->mDepthTexture);
    ostate->PopFramebufferBindings();
  }

  if (!mPrivate->mDrawHelper)
  {
    glsl::Preprocessor shaderPreprocessor;
    const auto shaderSourceStr =
      shaderPreprocessor.preprocess((std::string(ASSERT_PATH) + "CompositeFragmentShader.frag"));
    const auto shaderSource = shaderSourceStr.c_str();
    mPrivate->mDrawHelper =
      std::make_unique<vtkOpenGLQuadHelper>(renderWindow, nullptr, shaderSource, nullptr);
  }

  {
    ostate->PushFramebufferBindings();
    mPrivate->mFrameBuffer->Bind();
    auto program = mPrivate->mDrawHelper->Program;
    renderWindow->GetShaderCache()->ReadyShaderProgram(program);
    program->SetUniform2i("windowSize", windowSize);
    program->SetUniformi("maxLayer", 16);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_3D, mPrivate->mToolTexture->GetHandle());
    program->SetUniformi("toolTex", 0);
    program->SetUniform3f("gridOrigin", mPrivate->mWorkpieceOrigin);
    program->SetUniform3f("gridSize", mPrivate->mWorkpieceDimensions);
    program->SetUniform3f("gridSpacing", mPrivate->mWorkpieceSpacing);

    auto colorTexId = mPrivate->mColorTexture->GetHandle();
    /*glBindImageTexture(
      2, mPrivate->mHeadPointerImageTexture->GetHandle(), 0, GL_FALSE, 0, GL_READ_ONLY, GL_R32UI);*/
    mPrivate->mDrawHelper->Render();
    ostate->PopFramebufferBindings();
  }

  {
    ostate->PushFramebufferBindings();
    mPrivate->mFrameBuffer->Bind(vtkOpenGLFramebufferObject::GetReadMode());
    ostate->vtkglBlitFramebuffer(0, 0, windowSize[0], windowSize[1], 0, 0, windowSize[0],
      windowSize[1], GL_COLOR_BUFFER_BIT, GL_LINEAR);
    ostate->vtkglBlitFramebuffer(0, 0, windowSize[0], windowSize[1], 0, 0, windowSize[0],
      windowSize[1], GL_DEPTH_BUFFER_BIT, GL_NEAREST);
    ostate->PopFramebufferBindings();
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

void CompositeRenderPass::SetHeadPointerImage(vtkTextureObject* headPointerImage)
{
  mPrivate->mHeadPointerImageTexture = headPointerImage;
}

bool CompositeRenderPass::HasOpaqueDepthTexture() const
{
  return mPrivate->mOpaqueDepthTexture != nullptr;
}

bool CompositeRenderPass::HasHeadPointerImage() const
{
  return mPrivate->mHeadPointerImageTexture != nullptr;
}

bool CompositeRenderPass::HasToolTexture() const
{
  return mPrivate->mToolTexture != nullptr;
}

void CompositeRenderPass::SetWorkpieceParms(double* origin, double* spacing, int* dimensions)
{
  std::copy_n(origin, 3, mPrivate->mWorkpieceOrigin);
  std::copy_n(spacing, 3, mPrivate->mWorkpieceSpacing);
  mPrivate->mWorkpieceDimensions[0] = dimensions[0];
  mPrivate->mWorkpieceDimensions[1] = dimensions[1];
  mPrivate->mWorkpieceDimensions[2] = dimensions[2];
}
