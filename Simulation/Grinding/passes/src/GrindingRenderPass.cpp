#include "ABufferRenderPass.h"
#include "CompositeRenderPass.h"
#include "GrindingRenderPass.h"
#include "OpaqueDepthRenderPass.h"
#include "UpdateToolRenderPass.h"
#include <spdlog/spdlog.h>
#include <vtk_glad.h>
#include <vtkActorCollection.h>
#include <vtkImageData.h>
#include <vtkInformation.h>
#include <vtkInformationDoubleVectorKey.h>
#include <vtkInformationIntegerVectorKey.h>
#include <vtkMatrix4x4.h>
#include <vtkObjectFactory.h>
#include <vtkOpenGLFramebufferObject.h>
#include <vtkOpenGLRenderer.h>
#include <vtkOpenGLRenderWindow.h>
#include <vtkOpenGLState.h>
#include <vtkRenderState.h>
#include <vtkSmartPointer.h>
#include <vtkTextureObject.h>

vtkInformationKeyMacro(GrindingRenderPass, SpacingInfo, DoubleVector);
vtkInformationKeyMacro(GrindingRenderPass, DimensionsInfo, IntegerVector);
vtkInformationKeyMacro(GrindingRenderPass, OriginInfo, DoubleVector);

struct GrindingRenderPass::Private
{
  vtkSmartPointer<OpaqueDepthRenderPass> mOpaqueDepthRenderPass;
  vtkSmartPointer<UpdateToolRenderPass> mUpdateToolRenderPass;
  vtkSmartPointer<ABufferRenderPass> mABufferRenderPass;
  vtkSmartPointer<CompositeRenderPass> mCompositeRenderPass;
  Grinding::GrindingTool mGrindingTool;
  vtkSmartPointer<vtkActorCollection> mWorkpieceActors;
  vtkSmartPointer<vtkOpenGLFramebufferObject> mRenderFrameBuffer;
  vtkSmartPointer<vtkTextureObject> mColorAttachment;
  vtkSmartPointer<vtkTextureObject> mDepthAttachment;
  vtkSmartPointer<vtkTextureObject> mToolTex;
};

vtkStandardNewMacro(GrindingRenderPass);

void GrindingRenderPass::Render(const vtkRenderState* s)
{
  auto renderer = vtkOpenGLRenderer::SafeDownCast(s->GetRenderer());
  auto renderWindow = vtkOpenGLRenderWindow::SafeDownCast(renderer->GetRenderWindow());
  int* windowSize = renderWindow->GetSize();
  auto ostate = renderer->GetState();

  if (!mPrivate->mWorkpieceActors)
  {
    SPDLOG_ERROR("workpiece actors should set before");
    return;
  }

  if (!mPrivate->mOpaqueDepthRenderPass)
  {
    mPrivate->mOpaqueDepthRenderPass = vtkSmartPointer<OpaqueDepthRenderPass>::New();
    mPrivate->mOpaqueDepthRenderPass->SetOpaqueActors(mPrivate->mWorkpieceActors);
  }

  if (!mPrivate->mColorAttachment)
  {
    mPrivate->mColorAttachment = vtkSmartPointer<vtkTextureObject>::New();
    mPrivate->mColorAttachment->SetContext(renderWindow);
    mPrivate->mColorAttachment->Allocate2D(windowSize[0], windowSize[1], 4, VTK_UNSIGNED_CHAR, 0);
  }
  else
  {
    mPrivate->mColorAttachment->Resize(windowSize[0], windowSize[1]);
  }

  if (!mPrivate->mDepthAttachment)
  {
    mPrivate->mDepthAttachment = vtkSmartPointer<vtkTextureObject>::New();
    mPrivate->mDepthAttachment->SetContext(renderWindow);

    auto dbits = renderWindow->GetDepthBufferSize();

    if (renderWindow->GetStencilCapable())
    {
      mPrivate->mDepthAttachment->AllocateDepthStencil(windowSize[0], windowSize[1]);
    }
    else if (dbits == 32)
    {
      mPrivate->mDepthAttachment->AllocateDepth(
        windowSize[0], windowSize[1], vtkTextureObject::Fixed32);
    }
    else
    {
      mPrivate->mDepthAttachment->AllocateDepth(
        windowSize[0], windowSize[1], vtkTextureObject::Fixed24);
    }
  }
  else
  {
    mPrivate->mDepthAttachment->Resize(windowSize[0], windowSize[1]);
  }

  if (!mPrivate->mToolTex)
  {
    mPrivate->mToolTex = vtkSmartPointer<vtkTextureObject>::New();
    mPrivate->mToolTex->SetContext(renderWindow);
    mPrivate->mWorkpieceActors->InitTraversal();
    auto workpiece = mPrivate->mWorkpieceActors->GetNextItem();
    auto informations = workpiece->GetPropertyKeys();
    if (!informations)
    {
      SPDLOG_ERROR("workpiece has't a vtkInformation object");
      return;
    }
    auto hasDimensionsInfo = informations->Has(GrindingRenderPass::DimensionsInfo());
    if (!hasDimensionsInfo)
    {
      SPDLOG_ERROR("workpiece setting error, DimensionsInfo is nullptr: {}", hasDimensionsInfo);
      return;
    }

    auto* dimensions = informations->Get(GrindingRenderPass::DimensionsInfo());

    std::unique_ptr<float[]> rawData(new float[dimensions[0] * dimensions[1] * dimensions[2]]);
    std::fill_n(rawData.get(), dimensions[0] * dimensions[1] * dimensions[2],
      std::numeric_limits<float>::max());

    mPrivate->mToolTex->Create3DFromRaw(
      dimensions[0], dimensions[1], dimensions[2], 1, VTK_FLOAT, static_cast<void*>(rawData.get()));
    mPrivate->mToolTex->SetWrapS(vtkTextureObject::Repeat);
    mPrivate->mToolTex->SetWrapT(vtkTextureObject::Repeat);
  }

  mPrivate->mWorkpieceActors->InitTraversal();
  auto workpiece = mPrivate->mWorkpieceActors->GetNextActor();
  auto information = workpiece->GetPropertyKeys();

  auto hasOriginInfo = information->Has(GrindingRenderPass::OriginInfo());
  auto hasSpacingInfo = information->Has(GrindingRenderPass::SpacingInfo());
  auto hasDimsInfo = information->Has(GrindingRenderPass::DimensionsInfo());
  if (!hasOriginInfo || !hasSpacingInfo || !hasDimsInfo)
  {
    SPDLOG_ERROR(
      "workpiece information is incomplete, originInfo: {}, spacingInfo: {}, dimensionsInfo: {}",
      hasOriginInfo, hasSpacingInfo, hasDimsInfo);
    return;
  }
  auto origin = information->Get(GrindingRenderPass::OriginInfo());
  auto spacing = information->Get(GrindingRenderPass::SpacingInfo());
  auto dimensions = information->Get(GrindingRenderPass::DimensionsInfo());

  if (!mPrivate->mUpdateToolRenderPass)
  {
    mPrivate->mUpdateToolRenderPass = vtkSmartPointer<UpdateToolRenderPass>::New();
    mPrivate->mUpdateToolRenderPass->SetWorkpieceParams(origin, spacing, dimensions);
    mPrivate->mUpdateToolRenderPass->SetTool(Grinding::GrindingTool::Sphere, mPrivate->mToolTex);
    auto toolMatrix = vtkSmartPointer<vtkMatrix4x4>::New();
    toolMatrix->Identity();
    toolMatrix->SetElement(0, 3, origin[0]);
    toolMatrix->SetElement(1, 3, origin[1]);
    toolMatrix->SetElement(2, 3, origin[2]);
    mPrivate->mUpdateToolRenderPass->UpdateToolMatrix(toolMatrix);
  }

  if (!mPrivate->mABufferRenderPass)
  {
    mPrivate->mABufferRenderPass = vtkSmartPointer<ABufferRenderPass>::New();
    mPrivate->mABufferRenderPass->SetRenderActorCollection(mPrivate->mWorkpieceActors);
  }
  if (!mPrivate->mCompositeRenderPass)
  {
    mPrivate->mCompositeRenderPass = vtkSmartPointer<CompositeRenderPass>::New();
    mPrivate->mCompositeRenderPass->SetToolTexture(mPrivate->mToolTex);
    mPrivate->mCompositeRenderPass->SetWorkpieceParms(origin, spacing, dimensions);
  }

  if (!mPrivate->mRenderFrameBuffer)
  {
    mPrivate->mRenderFrameBuffer = vtkSmartPointer<vtkOpenGLFramebufferObject>::New();
    mPrivate->mRenderFrameBuffer->SetContext(renderWindow);
    ostate->PushFramebufferBindings();
    mPrivate->mRenderFrameBuffer->Bind();
    mPrivate->mRenderFrameBuffer->AddColorAttachment(0, mPrivate->mColorAttachment);
    mPrivate->mRenderFrameBuffer->AddDepthAttachment(mPrivate->mDepthAttachment);
    ostate->PopFramebufferBindings();
  }

  this->UpdateCamera(renderer);
  this->UpdateLightGeometry(renderer);
  this->UpdateLights(renderer);
  {
    ostate->PushFramebufferBindings();
    vtkOpenGLState::ScopedglClearColor clearColorSave(ostate);
    ostate->vtkglClearColor(0, 0, 0, 1);
    mPrivate->mRenderFrameBuffer->Bind(vtkOpenGLFramebufferObject::GetDrawMode());
    mPrivate->mOpaqueDepthRenderPass->Render(s);
    mPrivate->mUpdateToolRenderPass->Render(s);
    mPrivate->mABufferRenderPass->Render(s);

    if (!mPrivate->mCompositeRenderPass->HasHeadPointerImage())
    {
      mPrivate->mCompositeRenderPass->SetHeadPointerImage(
        mPrivate->mABufferRenderPass->GetHeadPointerImage());
    }
    if (!mPrivate->mCompositeRenderPass->HasToolTexture())
    {
      mPrivate->mCompositeRenderPass->SetToolTexture(mPrivate->mToolTex);
    }
    if (!mPrivate->mCompositeRenderPass->HasOpaqueDepthTexture())
    {
      mPrivate->mCompositeRenderPass->SetOpaqueDepthTexture(
        mPrivate->mOpaqueDepthRenderPass->GetOpaqueDepthTexture());
    }
    mPrivate->mCompositeRenderPass->Render(s);

    ostate->PopFramebufferBindings();
  }

  {
    ostate->PushFramebufferBindings();
    mPrivate->mRenderFrameBuffer->Bind(vtkOpenGLFramebufferObject::GetReadMode());
    //vtkOpenGLState::ScopedglEnableDisable stencialSaver(ostate, GL_STENCIL_TEST);
    //vtkOpenGLState::ScopedglEnableDisable depthSaver(ostate, GL_DEPTH_TEST);
    //vtkOpenGLState::ScopedglDepthMask depthMaskSaver(ostate);
    //ostate->vtkglDisable(GL_STENCIL_TEST);
    //ostate->vtkglDisable(GL_DEPTH_TEST);
    //ostate->vtkglDepthMask(GL_FALSE);
	
    ostate->vtkglBlitFramebuffer(0, 0, windowSize[0], windowSize[1], 0, 0, windowSize[0],
      windowSize[1], GL_COLOR_BUFFER_BIT, GL_LINEAR);
    ostate->vtkglBlitFramebuffer(0, 0, windowSize[0], windowSize[1], 0, 0, windowSize[0],
      windowSize[1], GL_DEPTH_BUFFER_BIT, GL_NEAREST);
    ostate->PopFramebufferBindings();
  }
}

void GrindingRenderPass::ReleaseGraphicsResources(vtkWindow* w)
{
  Superclass::ReleaseGraphicsResources(w);
  if (!mPrivate->mOpaqueDepthRenderPass)
  {
    mPrivate->mOpaqueDepthRenderPass->ReleaseGraphicsResources(w);
  }
}

void GrindingRenderPass::SetTool(Grinding::GrindingTool tool)
{
  mPrivate->mGrindingTool = tool;
}

void GrindingRenderPass::SetWorkpieceActors(vtkActorCollection* workpieces)
{
  mPrivate->mWorkpieceActors = workpieces;
}

GrindingRenderPass::GrindingRenderPass()
{
  mPrivate = std::make_unique<Private>();
}

GrindingRenderPass::~GrindingRenderPass() {}
