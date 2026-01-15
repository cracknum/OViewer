#include "preprocess.h"
#include "UpdateToolRenderPass.h"
#include <spdlog/spdlog.h>
#include <vtk_glad.h>
#include <vtkMatrix4x4.h>
#include <vtkObjectFactory.h>
#include <vtkOpenGLFramebufferObject.h>
#include <vtkOpenGLRenderWindow.h>
#include <vtkSmartPointer.h>
#include <vtkTextureObject.h>

#include <algorithm>
#include <vtkImageData.h>
#include <vtkOpenGLError.h>
#include <vtkOpenGLRenderer.h>
#include <vtkOpenGLState.h>
#include <vtkRenderState.h>

#if defined(GRINDING_UPDATE_TOOL_RENDER_PASS_DEBUG)
#include <vtkNIFTIImageWriter.h>
#endif //GRINDING_UPDATE_TOOL_RENDER_PASS_DEBUG

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
  GLuint mToolUpdateProgram;
  std::unique_ptr<WorkpieceParams> mWorkpieceParams;
  float mToolSize[3];

  Private()
    : mGrindingTool(Grinding::GrindingTool::None)
    , mToolUpdateProgram(0)
    , mToolSize{ 3, 3, 3 }
  {
    mToolMatrix = vtkSmartPointer<vtkMatrix4x4>::New();
    mToolMatrix->Identity();
    mToolInverseMatrix = vtkSmartPointer<vtkMatrix4x4>::New();
    vtkMatrix4x4::Invert(mToolMatrix, mToolInverseMatrix);
  }

  ~Private()
  {
    if (mToolUpdateProgram)
    {
      glDeleteProgram(mToolUpdateProgram);
    }
  }
};

vtkStandardNewMacro(UpdateToolRenderPass);

void UpdateToolRenderPass::Render(const vtkRenderState* s)
{
  vtkOpenGLClearErrorMacro();
  auto renderer = vtkOpenGLRenderer::SafeDownCast(s->GetRenderer());
  auto renderWindow = vtkOpenGLRenderWindow::SafeDownCast(renderer->GetRenderWindow());
  renderWindow->MakeCurrent();
  if (!mPrivate->mToolTex)
  {
    SPDLOG_ERROR("no tool texture set");
    return;
  }
  if (mPrivate->mGrindingTool == Grinding::GrindingTool::None)
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
    mPrivate->mToolUpdateProgram = glCreateProgram();
    auto shaderPreProcessor = glsl::Preprocessor();
    const auto computeShader = glCreateShader(GL_COMPUTE_SHADER);
	const auto computeShaderPath = std::string(ASSERT_PATH) + "UpdateToolComputeShader.comp";
    const auto computeShaderSource = shaderPreProcessor.preprocess(computeShaderPath);
    const auto computeShaderSourceStr = computeShaderSource.c_str();
    glShaderSource(computeShader, 1, &computeShaderSourceStr, nullptr);
    glCompileShader(computeShader);
    GLint status = -1;
    glGetShaderiv(computeShader, GL_COMPILE_STATUS, &status);
    if (status != GL_TRUE)
    {
      PrintShaderLog(computeShader);
      SPDLOG_ERROR(computeShaderSource);
      return;
    }
    glAttachShader(mPrivate->mToolUpdateProgram, computeShader);
    glLinkProgram(mPrivate->mToolUpdateProgram);
    glGetProgramiv(mPrivate->mToolUpdateProgram, GL_LINK_STATUS, &status);
    if (status != GL_TRUE)
    {
      PrintProgramLog(mPrivate->mToolUpdateProgram);
      return;
    }
  }
  glUseProgram(mPrivate->mToolUpdateProgram);
  glUniform1i(
    glGetUniformLocation(mPrivate->mToolUpdateProgram, "toolType"), mPrivate->mGrindingTool);
  float toolInverseMatrix[16]{};
  GetFloatMatrix4x4(mPrivate->mToolInverseMatrix, toolInverseMatrix);
  glUniformMatrix4fv(glGetUniformLocation(mPrivate->mToolUpdateProgram, "toolInverseMatrix"), 1,
    GL_FALSE, toolInverseMatrix);

  glUniform3f(glGetUniformLocation(mPrivate->mToolUpdateProgram, "toolSize"),
    mPrivate->mToolSize[0], mPrivate->mToolSize[1], mPrivate->mToolSize[2]);

  glUniform3f(glGetUniformLocation(mPrivate->mToolUpdateProgram, "gridOrigin"),
    mPrivate->mWorkpieceParams->mOrigin[0], mPrivate->mWorkpieceParams->mOrigin[1],
    mPrivate->mWorkpieceParams->mOrigin[2]);

  glUniform3f(glGetUniformLocation(mPrivate->mToolUpdateProgram, "gridSpacing"),
    mPrivate->mWorkpieceParams->mSpacing[0], mPrivate->mWorkpieceParams->mSpacing[1],
    mPrivate->mWorkpieceParams->mSpacing[2]);
  glUniform3f(glGetUniformLocation(mPrivate->mToolUpdateProgram, "gridDims"),
    mPrivate->mWorkpieceParams->mDimensions[0], mPrivate->mWorkpieceParams->mDimensions[1],
    mPrivate->mWorkpieceParams->mDimensions[2]);
  glBindImageTexture(0, mPrivate->mToolTex->GetHandle(), 0, GL_TRUE, 0, GL_READ_WRITE, GL_R32F);
  const unsigned int blockSize[3] = { (mPrivate->mToolTex->GetWidth() + 7) / 8,
    (mPrivate->mToolTex->GetHeight() + 7) / 8, (mPrivate->mToolTex->GetDepth() + 7) / 8 };
  glDispatchCompute(blockSize[0], blockSize[1], blockSize[2]);
  glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
  glUseProgram(0);
#if defined(GRINDING_UPDATE_TOOL_RENDER_PASS_DEBUG)

  auto texSize = mPrivate->mToolTex->GetWidth() *
    mPrivate->mToolTex->GetHeight() * mPrivate->mToolTex->GetDepth();
  std::unique_ptr<float[]> tex(new float[texSize]);

  glGetTexImage(GL_TEXTURE_3D, 0, GL_RED, GL_FLOAT, static_cast<void*>(tex.get()));

  vtkNew<vtkImageData> texImageData;
  texImageData->SetDimensions(mPrivate->mToolTex->GetWidth(), mPrivate->mToolTex->GetHeight(), mPrivate->mToolTex->GetDepth());
  texImageData->AllocateScalars(VTK_FLOAT, 1);
  auto scalarPointer = static_cast<float*>(texImageData->GetScalarPointer());
  std::copy_n(tex.get(), texSize, scalarPointer);
  vtkNew<vtkNIFTIImageWriter> writer;
  writer->SetInputData(texImageData);
  writer->SetFileName("texImageData.nii.gz");
  writer->Write();
#endif // GRINDING_UPDATE_TOOL_RENDER_PASS_DEBUG
}

void UpdateToolRenderPass::ReleaseGraphicsResources(vtkWindow* w)
{
  Superclass::ReleaseGraphicsResources(w);
}

void UpdateToolRenderPass::SetTool(
  const Grinding::GrindingTool grindingTool, vtkTextureObject* toolTex)
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

UpdateToolRenderPass::~UpdateToolRenderPass() = default;
void UpdateToolRenderPass::GetFloatMatrix4x4(const vtkMatrix4x4* matrix, float fMatrix[16])
{
  for (int i = 0; i < 4; ++i)
  {
    for (int j = 0; j < 4; j++)
    {
      fMatrix[i * 4 + j] = static_cast<float>(matrix->GetElement(i, j));
    }
  }
}
