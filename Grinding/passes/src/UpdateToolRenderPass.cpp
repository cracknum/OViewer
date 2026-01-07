#include "UpdateToolRenderPass.h"
#include "preprocess.h"
#include <spdlog/spdlog.h>
#include <vtkMatrix4x4.h>
#include <vtkObjectFactory.h>
#include <vtkOpenGLFramebufferObject.h>
#include <vtkOpenGLRenderWindow.h>
#include <vtkShader.h>
#include <vtkShaderProgram.h>
#include <vtkSmartPointer.h>
#include <vtkTextureObject.h>
#include <vtk_glad.h>

#include <algorithm>
#include <vtkOpenGLRenderer.h>
#include <vtkOpenGLShaderCache.h>
#include <vtkRenderState.h>

struct WorkpieceParams
{
  float mOrigin[3];
  float mSpacing[3];
  float mDimensions[3];
  WorkpieceParams(const double* origin, const double* spacing, const int* dims)
    : mOrigin{ static_cast<float>(origin[0]), static_cast<float>(origin[1]),
      static_cast<float>(origin[2]) }
    , mSpacing{ static_cast<float>(spacing[0]), static_cast<float>(spacing[1]),
      static_cast<float>(spacing[2]) }
    , mDimensions{ static_cast<float>(dims[0]), static_cast<float>(dims[1]),
      static_cast<float>(dims[2]) }
  {
  }
};

struct UpdateToolRenderPass::Private
{
  vtkSmartPointer<vtkTextureObject> mToolTex;
  vtkSmartPointer<vtkMatrix4x4> mToolMatrix;
  vtkSmartPointer<vtkMatrix4x4> mToolInverseMatrix;
  Grinding::GrindingTool mGrindingTool;
  vtkSmartPointer<vtkOpenGLFramebufferObject> mFrameBuffer;
  vtkSmartPointer<vtkShaderProgram> mToolUpdateProgram;
  std::unique_ptr<WorkpieceParams> mWorkpieceParams;
  float mToolSize[3];

  Private()
    : mGrindingTool(Grinding::GrindingTool::None)
    , mToolSize{ 0.5, 0.5, 0.5 }
  {
  }
};

vtkStandardNewMacro(UpdateToolRenderPass);

void UpdateToolRenderPass::Render(const vtkRenderState* s)
{
  auto renderer = vtkOpenGLRenderer::SafeDownCast(s->GetRenderer());
  auto renderWindow = vtkOpenGLRenderWindow::SafeDownCast(renderer->GetRenderWindow());
  if (!mPrivate->mToolTex)
  {
    SPDLOG_ERROR("no tool texture set");
    return;
  }
  if (!mPrivate->mGrindingTool == Grinding::GrindingTool::None)
  {
    SPDLOG_ERROR("no tool type set");
    return;
  }

  if (mPrivate->mToolMatrix == nullptr)
  {
    SPDLOG_ERROR("no tool matrix set");
    return;
  }
  if (!mPrivate->mToolUpdateProgram)
  {
    mPrivate->mToolUpdateProgram = vtkSmartPointer<vtkShaderProgram>::New();
    auto computeShader = vtkSmartPointer<vtkShader>::New();
    auto shaderPreProcessor = glsl::Preprocessor();
    auto computeSource = shaderPreProcessor.preprocess("../shader/UpdateToolComputeShader.comp");
    computeShader->SetSource(computeSource);
    mPrivate->mToolUpdateProgram->SetComputeShader(computeShader);
  }

  renderWindow->GetShaderCache()->ReadyShaderProgram(mPrivate->mToolUpdateProgram);
  mPrivate->mToolUpdateProgram->SetUniformi("toolType", mPrivate->mGrindingTool);
  float toolInverseMatrix[16]{};
  GetFloatMatrix4x4(mPrivate->mToolInverseMatrix, toolInverseMatrix);
  mPrivate->mToolUpdateProgram->SetUniformMatrix4x4("toolInverseMatrix", toolInverseMatrix);

  mPrivate->mToolUpdateProgram->SetUniform3f("toolSize", mPrivate->mToolSize);

  mPrivate->mToolUpdateProgram->SetUniform3f("gridOrigin", mPrivate->mWorkpieceParams->mOrigin);
  mPrivate->mToolUpdateProgram->SetUniform3f("gridSpacing", mPrivate->mWorkpieceParams->mSpacing);
  mPrivate->mToolUpdateProgram->SetUniform3f("gridDims", mPrivate->mWorkpieceParams->mDimensions);
  glBindImageTexture(0, mPrivate->mToolTex->GetHandle(), 0, GL_TRUE, GL_READ_WRITE, 0, GL_R32F);
  const unsigned int blockSize[3] = { (mPrivate->mToolTex->GetWidth() + 7) / 8,
    (mPrivate->mToolTex->GetHeight() + 7) / 8, (mPrivate->mToolTex->GetDepth() + 7) / 8 };
  glDispatchCompute(blockSize[0], blockSize[1], blockSize[2]);
  glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
}

void UpdateToolRenderPass::ReleaseGraphicsResources(vtkWindow* w)
{
  Superclass::ReleaseGraphicsResources(w);
}

void UpdateToolRenderPass::SetTool(Grinding::GrindingTool grindingTool, vtkTextureObject* toolTex)
{
  mPrivate->mToolTex = toolTex;
  mPrivate->mGrindingTool = grindingTool;
}

void UpdateToolRenderPass::UpdateToolMatrix(vtkMatrix4x4* matrix)
{
  mPrivate->mToolMatrix = matrix;
  if (!mPrivate->mToolInverseMatrix)
  {
    mPrivate->mToolInverseMatrix = vtkSmartPointer<vtkMatrix4x4>::New();
    mPrivate->mToolInverseMatrix->Identity();
  }
  vtkMatrix4x4::Invert(matrix, mPrivate->mToolInverseMatrix);
}
void UpdateToolRenderPass::SetWorkpieceParams(double* origin, double* spacing, int* dimensions)
{
  if (!mPrivate->mWorkpieceParams)
  {
    mPrivate->mWorkpieceParams = std::make_unique<WorkpieceParams>(origin, spacing, dimensions);
  }

  std::transform(origin, origin + 3, mPrivate->mWorkpieceParams->mOrigin,
    [](const double oValue) { return static_cast<float>(oValue); });
  std::transform(spacing, spacing + 3, mPrivate->mWorkpieceParams->mSpacing,
    [](const double oValue) { return static_cast<float>(oValue); });
  std::transform(dimensions, dimensions + 3, mPrivate->mWorkpieceParams->mDimensions,
    [](const int oValue) { return static_cast<float>(oValue); });
}

UpdateToolRenderPass::UpdateToolRenderPass()
{
  mPrivate = std::make_unique<Private>();
}

UpdateToolRenderPass::~UpdateToolRenderPass() {}
void UpdateToolRenderPass::GetFloatMatrix4x4(const vtkMatrix4x4* matrix, float fMatrix[16])
{
  for (int i = 0; i < 4; ++i)
  {
    for (int j = 0; j < 4; j++)
    {
      fMatrix[i * 4 + j] = matrix->GetElement(i, j);
    }
  }
}
