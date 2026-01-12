#include "CompositeRenderPass.h"
#include "preprocess.h"
#include <vtkObjectFactory.h>
#include <vtkOpenGLError.h>
#include <vtkOpenGLFramebufferObject.h>
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
  // outside inject
  vtkSmartPointer<vtkTextureObject> mHeadPointerImageTexture;
  GLuint mCProgram;

  Private()
    : mCProgram(0)
  {
  }
};

vtkStandardNewMacro(CompositeRenderPass);

void CompositeRenderPass::Render(const vtkRenderState* s)
{
  auto renderer = vtkOpenGLRenderer::SafeDownCast(s->GetRenderer());
  auto renderWindow = vtkOpenGLRenderWindow::SafeDownCast(renderer->GetRenderWindow());
  auto ostate = renderWindow->GetState();
  int* windowSize = renderWindow->GetSize();

  if (!mPrivate->mToolTexture || !mPrivate->mOpaqueDepthTexture || !mPrivate->mHeadPointerImageTexture)
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

  if (!mPrivate->mFrameBuffer)
  {
    mPrivate->mFrameBuffer = vtkSmartPointer<vtkOpenGLFramebufferObject>::New();
    mPrivate->mFrameBuffer->SetContext(renderWindow);
    ostate->PushFramebufferBindings();
    mPrivate->mFrameBuffer->Bind();
    mPrivate->mFrameBuffer->AddColorAttachment(0, mPrivate->mColorTexture);
    ostate->PopFramebufferBindings();
  }

  if (!mPrivate->mCProgram)
  {
    const auto* shaderPath = (std::string(ASSERT_PATH) + "CompositeCSShader.comp").c_str();
    glsl::Preprocessor shaderPreprocessor;
    const auto shaderSourceStr = shaderPreprocessor.preprocess(shaderPath);
    const auto shaderSource = shaderSourceStr.c_str();
    GLuint computeShader = glCreateShader(GL_COMPUTE_SHADER);
    glShaderSource(computeShader, 1, &shaderSource, nullptr);
    glCompileShader(computeShader);
    GLint status = -1;
    glGetShaderiv(computeShader, GL_COMPILE_STATUS, &status);
    if (status != GL_TRUE)
    {
      PrintShaderLog(computeShader);
      SPDLOG_ERROR(shaderSourceStr);
      return;
    }
    mPrivate->mCProgram = glCreateProgram();
    glAttachShader(mPrivate->mCProgram, computeShader);
    glLinkProgram(mPrivate->mCProgram);
    glGetProgramiv(mPrivate->mCProgram, GL_LINK_STATUS, &status);
    if (status != GL_TRUE)
    {
      PrintProgramLog(mPrivate->mCProgram);
      return;
    }
  }

  {
    ostate->PushFramebufferBindings();
    glUseProgram(mPrivate->mCProgram);
    glUniform2i(
      glGetUniformLocation(mPrivate->mCProgram, "windowSize"), windowSize[0], windowSize[1]);
    glUniform1i(glGetUniformLocation(mPrivate->mCProgram, "maxLayer"), 16);
    auto colorTexId = mPrivate->mColorTexture->GetHandle();
    glBindImageTexture(
      2, mPrivate->mHeadPointerImageTexture->GetHandle(), 0, GL_FALSE, 0, GL_READ_ONLY, GL_R32UI);
    glBindImageTexture(4, colorTexId, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA8);
    int xUnit = (windowSize[0] + 7) / 8;
    int yUnit = (windowSize[1] + 7) / 8;
    glDispatchCompute(xUnit, yUnit, 1);
    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
    glUseProgram(0);
    ostate->PopFramebufferBindings();
  }

  {
    ostate->PushFramebufferBindings();
    mPrivate->mFrameBuffer->Bind(vtkOpenGLFramebufferObject::GetReadMode());
    ostate->vtkglBlitFramebuffer(0, 0, windowSize[0], windowSize[1], 0, 0, windowSize[0],
      windowSize[1], GL_COLOR_BUFFER_BIT, GL_LINEAR);
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
