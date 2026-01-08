#include "ColorChangePass.h"
#include "GrindingRenderPass.h"
#include "OpaqueDepthRenderPass.h"
#include <iostream>
#include <spdlog/spdlog.h>
#include <vtkActor.h>
#include <vtkFlyingEdges3D.h>
#include <vtkImageData.h>
#include <vtkInformation.h>
#include <vtkInformationStringKey.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkNIFTIImageReader.h>
#include <vtkOpenGLRenderWindow.h>
#include <vtkOpenGLState.h>
#include <vtkOpenGLTexture.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRenderer.h>
#include <vtkRenderPassCollection.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkSequencePass.h>
#include <vtkSmartPointer.h>
#include <vtkSTLReader.h>
#include <vtkTextureObject.h>
#include <vtkTextureUnitManager.h>
#include <vtkWindowedSincPolyDataFilter.h>
#include "UpdateToolRenderPass.h"
#include <vtkTextureObject.h>
#include <memory>
#include <vtkMatrix4x4.h>

#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080

vtkSmartPointer<vtkActor> reconstructWorkpiece(vtkImageData* imageData)
{
  auto flyingEdgesAlgo = vtkSmartPointer<vtkFlyingEdges3D>::New();
  flyingEdgesAlgo->SetInputData(imageData);
  flyingEdgesAlgo->SetValue(0, 17);
  auto smoothPolyDataFilter = vtkSmartPointer<vtkWindowedSincPolyDataFilter>::New();
  smoothPolyDataFilter->SetInputConnection(flyingEdgesAlgo->GetOutputPort());
  smoothPolyDataFilter->SetNumberOfIterations(15);
  smoothPolyDataFilter->BoundarySmoothingOff();
  smoothPolyDataFilter->FeatureEdgeSmoothingOff();
  smoothPolyDataFilter->SetFeatureAngle(120.0);
  smoothPolyDataFilter->SetPassBand(.001);
  smoothPolyDataFilter->NonManifoldSmoothingOn();
  smoothPolyDataFilter->NormalizeCoordinatesOn();
  
  auto mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  mapper->SetInputConnection(smoothPolyDataFilter->GetOutputPort());
  auto actor = vtkSmartPointer<vtkActor>::New();
  actor->SetMapper(mapper);
  
  return actor;
}
int main()
{
  auto reader = vtkNIFTIImageReader::New();
  auto renderer = vtkSmartPointer<vtkRenderer>::New();
  auto renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
  auto renderWindowInteractor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
  auto renderWindowStyle = vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();
  renderWindow->AddRenderer(renderer);
  renderWindow->SetSize(1920, 1080);
  renderWindowInteractor->SetRenderWindow(renderWindow);
  renderWindowInteractor->SetInteractorStyle(renderWindowStyle);
  renderWindowInteractor->Initialize();

  reader->SetFileName(
    R"(F:\Workspace\Projects\OViewer\Grinding\tests\mask_tooth_crop.nii.gz)");
  reader->Update();
  auto workpieceData = reader->GetOutput();
  int dimensions[3]{};
  double origin[3]{};
  double spacing[3]{};
  workpieceData->GetDimensions(dimensions);
  workpieceData->GetOrigin(origin);
  workpieceData->GetSpacing(spacing);

  auto workpieceActor = reconstructWorkpiece(reader->GetOutput());
  renderer->AddActor(workpieceActor);
  auto information = vtkInformation::New();
  workpieceActor->SetPropertyKeys(information);
  information->FastDelete();
  information->Set(GrindingRenderPass::DimensionsInfo(), dimensions, 3);

  auto toolReader = vtkSmartPointer<vtkSTLReader>::New();
  toolReader->SetFileName(R"(F:\Workspace\Data\Yarn_Needle.STL)");
  toolReader->Update();
  auto toolMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  toolMapper->SetInputData(toolReader->GetOutput());
  auto toolActor = vtkSmartPointer<vtkActor>::New();
  toolActor->SetMapper(toolMapper);
  renderer->AddActor(toolActor);

  renderWindow->Initialize();

  auto toolTex = vtkSmartPointer<vtkTextureObject>::New();
  toolTex->SetContext(vtkOpenGLRenderWindow::SafeDownCast(renderWindow));
  std::unique_ptr<float[]> rawData(new float[dimensions[0] * dimensions[1] * dimensions[2]]);
  std::fill_n(rawData.get(), dimensions[0] * dimensions[1] * dimensions[2], std::numeric_limits<float>::max());
  toolTex->Create3DFromRaw(dimensions[0], dimensions[1], dimensions[2], 1, VTK_FLOAT, static_cast<void*>(rawData.get()));
  auto toolMatrix = vtkSmartPointer<vtkMatrix4x4>::New();
  toolMatrix->Identity();

  auto updateToolRenderPass = vtkSmartPointer<UpdateToolRenderPass>::New();
  updateToolRenderPass->SetWorkpieceParams(origin, spacing, dimensions);
  updateToolRenderPass->SetTool(Grinding::GrindingTool::Sphere, toolTex);
  updateToolRenderPass->UpdateToolMatrix(toolMatrix);

  renderer->SetPass(updateToolRenderPass);

  renderWindowInteractor->Start();
}
