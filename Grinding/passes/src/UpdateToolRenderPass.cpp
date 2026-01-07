#include "UpdateToolRenderPass.h"
#include <vtkObjectFactory.h>
#include <vtkSmartPointer.h>
#include <vtkTextureObject.h>
#include <spdlog/spdlog.h>
#include <vtkMatrix4x4.h>

struct UpdateToolRenderPass::Private
{
	vtkSmartPointer<vtkTextureObject> mToolTex;
	vtkSmartPointer<vtkMatrix4x4> mToolMatrix;
	Grinding::GrindingTool mGrindingTool;

	Private()
	:mGrindingTool(Grinding::GrindingTool::None)
	{
		
	}
};

vtkStandardNewMacro(UpdateToolRenderPass);

void UpdateToolRenderPass::Render(const vtkRenderState* s) {

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
}

UpdateToolRenderPass::UpdateToolRenderPass()
{
  mPrivate = std::make_unique<Private>();
}

UpdateToolRenderPass::~UpdateToolRenderPass() {}
