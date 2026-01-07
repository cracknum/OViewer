#include "OpaqueDepthRenderPass.h"
#include <vtk_glew.h>
#include <spdlog/spdlog.h>
#include <unordered_map>
#include <vtkCamera.h>
#include <vtkObjectFactory.h>
#include <vtkOpenGLActor.h>
#include <vtkOpenGLFramebufferObject.h>
#include <vtkOpenGLQuadHelper.h>
#include <vtkOpenGLRenderWindow.h>
#include <vtkOpenGLRenderer.h>
#include <vtkOpenGLShaderCache.h>
#include <vtkOpenGLState.h>
#include <vtkPolyData.h>
#include <vtkRenderState.h>
#include <vtkRenderer.h>
#include <vtkShaderProgram.h>
#include <vtkSmartPointer.h>
#include <vtkTextureObject.h>

struct OpaqueDepthRenderPass::Private
{
  vtkSmartPointer<vtkActorCollection> mOpaqueActors;
  vtkSmartPointer<vtkOpenGLFramebufferObject> mDepthFrameBuffer;
  vtkSmartPointer<vtkTextureObject> mDepthTexture;
  std::unique_ptr<vtkOpenGLQuadHelper> mHelperQuad;
};

vtkStandardNewMacro(OpaqueDepthRenderPass);

OpaqueDepthRenderPass::OpaqueDepthRenderPass()
{
  mPrivate = std::make_unique<Private>();
}

void OpaqueDepthRenderPass::Render(const vtkRenderState* s)
{
  auto renderer = vtkOpenGLRenderer::SafeDownCast(s->GetRenderer());

  if (!renderer)
  {
    SPDLOG_ERROR("OpaqueDepthRenderPass is not compatible with non-opengl renderer");
    return;
  }

  if (!mPrivate->mOpaqueActors)
  {
    SPDLOG_ERROR("setting workpiece before");
    return;
  }

  auto renderWindow = vtkOpenGLRenderWindow::SafeDownCast(renderer->GetRenderWindow());
  renderWindow->MakeCurrent();
  double viewportSize[4]{};
  renderer->GetViewport(viewportSize);
  int* windowSize = renderWindow->GetSize();
  auto ostate = renderer->GetState();

  if (!mPrivate->mDepthTexture)
  {
    mPrivate->mDepthTexture = vtkSmartPointer<vtkTextureObject>::New();
    mPrivate->mDepthTexture->SetContext(renderWindow);
    int dbits = renderWindow->GetDepthBufferSize();
    if (renderWindow->GetStencilCapable())
    {
      mPrivate->mDepthTexture->AllocateDepthStencil(windowSize[0], windowSize[1]);
    }
    else if (dbits == 32)
    {
      mPrivate->mDepthTexture->AllocateDepth(
        windowSize[0], windowSize[1], vtkTextureObject::Fixed32);
    }
    else
    {
      mPrivate->mDepthTexture->AllocateDepth(
        windowSize[0], windowSize[1], vtkTextureObject::Fixed24);
    }
  }
  else
  {
    mPrivate->mDepthTexture->Resize(windowSize[0], windowSize[1]);
  }

  if (!mPrivate->mDepthFrameBuffer)
  {
    mPrivate->mDepthFrameBuffer = vtkSmartPointer<vtkOpenGLFramebufferObject>::New();
    mPrivate->mDepthFrameBuffer->SetContext(renderWindow);
    ostate->PushFramebufferBindings();
    mPrivate->mDepthFrameBuffer->Bind();
    mPrivate->mDepthFrameBuffer->AddDepthAttachment(mPrivate->mDepthTexture);
    ostate->PopFramebufferBindings();
  }
  else
  {
    mPrivate->mDepthFrameBuffer->Resize(windowSize[0], windowSize[1]);
  }

  {
    ostate->PushFramebufferBindings();
    mPrivate->mDepthFrameBuffer->Bind(vtkOpenGLFramebufferObject::GetDrawMode());
    ostate->vtkglClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    ostate->vtkglClearDepth(1.0f);
    ostate->vtkglClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    vtkOpenGLState::ScopedglEnableDisable stencialSaver(ostate, GL_STENCIL_TEST);
    vtkOpenGLState::ScopedglEnableDisable depthSaver(ostate, GL_DEPTH_TEST);
    vtkOpenGLState::ScopedglColorMask colorMaskSaver(ostate);
    vtkOpenGLState::ScopedglDepthMask depthMaskSaver(ostate);

    ostate->vtkglDisable(GL_STENCIL_TEST);
    ostate->vtkglEnable(GL_DEPTH_TEST);
    ostate->vtkglDepthMask(GL_TRUE);
    ostate->vtkglColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

    this->UpdateCamera(renderer);
    this->UpdateLightGeometry(renderer);
    this->UpdateLights(renderer);
	vtkActor* opaqueActor = nullptr;
	mPrivate->mOpaqueActors->InitTraversal();
	while (opaqueActor = mPrivate->mOpaqueActors->GetNextActor())
	{
		opaqueActor->Render(renderer, opaqueActor->GetMapper());
	}
	
    ostate->PopFramebufferBindings();
  }

#ifdef GRINDING_OPAQUE_DEPTH_RENDER_PASS_DEBUG
  {
    ostate->PushFramebufferBindings();
    renderWindow->GetRenderFramebuffer()->Bind(vtkOpenGLFramebufferObject::GetDrawMode());
    vtkOpenGLState::ScopedglEnableDisable stencialSaver(ostate, GL_STENCIL_TEST);
    vtkOpenGLState::ScopedglEnableDisable depthSaver(ostate, GL_DEPTH_TEST);
	vtkOpenGLState::ScopedglDepthMask depthMaskSaver(ostate);
    ostate->vtkglDisable(GL_STENCIL_TEST);
    ostate->vtkglDisable(GL_DEPTH_TEST);
	ostate->vtkglDepthMask(GL_FALSE);

    this->RenderDepthTextureToColorTexture(renderer);

    ostate->PopFramebufferBindings();
  }
#endif // GRINDING_OPAQUE_DEPTH_RENDER_PASS_DEBUG

{
	ostate->PushFramebufferBindings();
	auto renderFrameBuffer = renderWindow->GetRenderFramebuffer();
	renderFrameBuffer->Bind(vtkOpenGLFramebufferObject::GetDrawMode());
	mPrivate->mDepthFrameBuffer->Bind(vtkOpenGLFramebufferObject::GetReadMode());
	vtkOpenGLState::ScopedglDepthMask depthMaskSaver(ostate);
	vtkOpenGLState::ScopedglEnableDisable depthTestSaver(ostate, GL_DEPTH_TEST);
	vtkOpenGLState::ScopedglColorMask colorMaskSaver(ostate);
	ostate->vtkglColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	ostate->vtkglDepthMask(GL_TRUE);
	ostate->vtkglClearDepth(1.0f);
	ostate->vtkglClear(GL_DEPTH_BUFFER_BIT);
	ostate->vtkglBlitFramebuffer(
		0, 0, windowSize[0], windowSize[1],
		0, 0, windowSize[0], windowSize[1],
		GL_DEPTH_BUFFER_BIT,
		GL_NEAREST
	);
	ostate->PopFramebufferBindings();
}
}

void OpaqueDepthRenderPass::ReleaseGraphicsResources(vtkWindow* w) {}

void OpaqueDepthRenderPass::SetOpaqueActors(vtkActorCollection* actors)
{
  mPrivate->mOpaqueActors = actors;
}

OpaqueDepthRenderPass::~OpaqueDepthRenderPass() {}

void OpaqueDepthRenderPass::RenderDepthTextureToColorTexture(vtkRenderer* renderer)
{
  auto renderWindow = vtkOpenGLRenderWindow::SafeDownCast(renderer->GetRenderWindow());
  constexpr const char* fs = R"(
		#version 150
		#ifndef GL_ES
		#define highp
		#define mediump
		#define lowp
		#define texelFetchBuffer texelFetch
		#endif // GL_ES
		#define attribute in
		#define varying out
		uniform sampler2D depthTexture;
		uniform float nearZ;
		uniform float farZ;
		in vec2 texCoord;
		out vec4 fragColor;

		float linearizeDepth(float depth)
		{
			float linearDepth = (nearZ * farZ) / (farZ - depth * (farZ - nearZ));
			return abs(linearDepth);
		}

		void main()
		{
			float depth = linearizeDepth(texture(depthTexture, texCoord).r);
			float normalizedDepth = (depth - nearZ) / (farZ - nearZ);
    		normalizedDepth = clamp(normalizedDepth, 0.0, 1.0);
			fragColor = vec4(vec3(normalizedDepth), 1.0);
		}
	
	)";
  if (!mPrivate->mHelperQuad)
  {
    mPrivate->mHelperQuad =
      std::make_unique<vtkOpenGLQuadHelper>(renderWindow, nullptr, fs, nullptr);
    if (!mPrivate->mHelperQuad->Program)
    {
      SPDLOG_ERROR("failed to compiled depth visualization shader");
      return;
    }
  }

  auto camera = renderer->GetActiveCamera();
  double clipRange[2]{};
  camera->GetClippingRange(clipRange);
  renderWindow->GetShaderCache()->ReadyShaderProgram(mPrivate->mHelperQuad->Program);
  mPrivate->mHelperQuad->Program->SetUniformf("nearZ", clipRange[0]);
  mPrivate->mHelperQuad->Program->SetUniformf("farZ", clipRange[1]);
  mPrivate->mHelperQuad->Program->SetUniformi("depthTexture", 0);
  mPrivate->mDepthTexture->Activate();
  mPrivate->mHelperQuad->Render();

  mPrivate->mDepthTexture->Deactivate();
}
