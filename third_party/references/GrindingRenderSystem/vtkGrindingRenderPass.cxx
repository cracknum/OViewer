#include "vtkGrindingRenderPass.h"
#include <vtkObjectFactory.h>
#include <vtkOpenGLRenderWindow.h>
#include <vtkOpenGLShaderCache.h>
#include <vtkShaderProgram.h>
#include <vtk_glew.h>
#include <vtkMatrix4x4.h>
#include <vtkCamera.h>
#include <vtkRenderer.h>

vtkStandardNewMacro(vtkGrindingRenderPass);

namespace {
const char* vertexCode = R"(
//VTK::System::Dec
in vec2 vertexMC;
out vec2 tcoordVCVSOutput;
void main()
{
  tcoordVCVSOutput = vertexMC * 0.5 + 0.5;
  gl_Position = vec4(vertexMC, 0.0, 1.0);
}
)";

const char* fragmentCode = R"(
#version 430 core
//YHQ Composite fs
//VTK::System::Dec
uniform sampler2D colorSampler;
uniform sampler2D depthSampler;
uniform sampler3D removalVolume;
uniform mat4 inverseProjectionMatrix;
uniform mat4 inverseViewMatrix;
uniform vec3 volMin;
uniform vec3 volMax;
in vec2 tcoordVCVSOutput;
//VTK::Output::Dec
void main()
{
  //VTK::FSQ::Impl
}
)";

const char* fsqImpl = R"(
vec2 texCoord = tcoordVCVSOutput;
float depth = texture(depthSampler, texCoord).r;
if (depth >= 1.0) {
  gl_FragData[0] = texture(colorSampler, texCoord);
  return;
}
vec4 clipPos = vec4(texCoord * 2.0 - 1.0, depth * 2.0 - 1.0, 1.0);
vec4 viewPos = inverseProjectionMatrix * clipPos;
viewPos /= viewPos.w;
vec4 worldPos = inverseViewMatrix * viewPos;
vec3 volUVW = (worldPos.xyz - volMin) / (volMax - volMin);
volUVW = clamp(volUVW, 0.0, 1.0);
float removed = texture(removalVolume, volUVW).r;
if (removed > 0.5) {
  gl_FragData[0] = vec4(1.0, 0.0, 0.0, 0.7) * texture(colorSampler, texCoord);
} else {
  gl_FragData[0] = texture(colorSampler, texCoord);
}
)";
}

vtkGrindingRenderPass::vtkGrindingRenderPass() = default;
vtkGrindingRenderPass::~vtkGrindingRenderPass()
{
  if (this->FullScreenVAO)
  {
    glDeleteVertexArrays(1, &this->FullScreenVAO);
    glDeleteBuffers(1, &this->FullScreenVBO);
  }
}

void vtkGrindingRenderPass::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os, indent);
}

void vtkGrindingRenderPass::SetSceneColorTexture(vtkTextureObject* tex)
{
  this->SceneColorTexture = tex;
}

void vtkGrindingRenderPass::SetSceneDepthTexture(vtkTextureObject* tex)
{
  this->SceneDepthTexture = tex;
}

void vtkGrindingRenderPass::SetRemovalVolume(vtkTextureObject* volume)
{
  this->RemovalVolume = volume;
}

void vtkGrindingRenderPass::PrepareFullScreenQuad(vtkOpenGLRenderWindow* renWin)
{
  if (this->QuadPrepared) return;

  float quadVertices[] = {
    -1.0f, -1.0f,
     1.0f, -1.0f,
    -1.0f,  1.0f,
     1.0f,  1.0f
  };

  glGenVertexArrays(1, &this->FullScreenVAO);
  glGenBuffers(1, &this->FullScreenVBO);

  glBindVertexArray(this->FullScreenVAO);
  glBindBuffer(GL_ARRAY_BUFFER, this->FullScreenVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
  glBindVertexArray(0);

  this->QuadPrepared = true;
}

void vtkGrindingRenderPass::Render(const vtkRenderState* s)
{
  vtkOpenGLRenderWindow* renWin = vtkOpenGLRenderWindow::SafeDownCast(s->GetRenderWindow());
  if (!renWin) return;

  this->PrepareFullScreenQuad(renWin);

  GLuint prevFBO = 0;
  glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, (GLint*)&prevFBO);

  vtkShaderProgram* prog = renWin->GetShaderCache()->ReadyShaderProgram(
    vertexCode, fragmentCode, nullptr);
  prog->Substitute("//VTK::FSQ::Impl", fsqImpl);

  if (this->SceneColorTexture) this->SceneColorTexture->Activate();
  if (this->SceneDepthTexture) this->SceneDepthTexture->Activate();
  if (this->RemovalVolume) this->RemovalVolume->Activate();

  prog->SetUniformi("colorSampler", this->SceneColorTexture ? this->SceneColorTexture->GetTextureUnit() : 0);
  prog->SetUniformi("depthSampler", this->SceneDepthTexture ? this->SceneDepthTexture->GetTextureUnit() : 1);
  prog->SetUniformi("removalVolume", this->RemovalVolume ? this->RemovalVolume->GetTextureUnit() : 2);

  vtkRenderer* ren = s->GetRenderer();
  if (ren)
  {
    vtkMatrix4x4* proj = ren->GetActiveCamera()->GetProjectionTransformMatrix(ren->GetTiledAspectRatio(), 0, 1);
    vtkMatrix4x4* view = ren->GetActiveCamera()->GetViewTransformMatrix();

    vtkMatrix4x4* invProj = vtkMatrix4x4::New();
    vtkMatrix4x4* invView = vtkMatrix4x4::New();
    vtkMatrix4x4::Invert(proj, invProj);
    vtkMatrix4x4::Invert(view, invView);

    prog->SetUniformMatrix("inverseProjectionMatrix", invProj);
    prog->SetUniformMatrix("inverseViewMatrix", invView);

    invProj->Delete();
    invView->Delete();
  }

  prog->SetUniform3f("volMin", this->VolumeMin);
  prog->SetUniform3f("volMax", this->VolumeMax);

  this->RenderFullScreenQuad(renWin, s);

  if (this->RemovalVolume) this->RemovalVolume->Deactivate();
  if (this->SceneDepthTexture) this->SceneDepthTexture->Deactivate();
  if (this->SceneColorTexture) this->SceneColorTexture->Deactivate();

  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, prevFBO);
}

void vtkGrindingRenderPass::RenderFullScreenQuad(vtkOpenGLRenderWindow* renWin, const vtkRenderState* s)
{
  glBindVertexArray(this->FullScreenVAO);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
  glBindVertexArray(0);
}