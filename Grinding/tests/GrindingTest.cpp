#include "ColorChangePass.h"
#include "OpaqueDepthRenderPass.h"
#include <iostream>
#include <vtkActor.h>
#include <vtkFlyingEdges3D.h>
#include <vtkImageData.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkNIFTIImageReader.h>
#include <vtkOpenGLRenderWindow.h>
#include <vtkOpenGLState.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRenderPassCollection.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkSTLReader.h>
#include <vtkSequencePass.h>
#include <vtkSmartPointer.h>
#include <vtkTextureObject.h>
#include <vtkTextureUnitManager.h>
#include <vtkWindowedSincPolyDataFilter.h>
#include <vtkOpenGLTexture.h>
#include <vtkInformationStringKey.h>
#include <spdlog/spdlog.h>

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
    R"(D:\Workspace\gitProject\build\bin\Release\data\1\coarsePredict\17\mask_tooth_crop.nii.gz)");
  reader->Update();

  auto workpieceActor = reconstructWorkpiece(reader->GetOutput());
  renderer->AddActor(workpieceActor);

  auto toolReader = vtkSmartPointer<vtkSTLReader>::New();
  toolReader->SetFileName(R"(D:\Workspace\gitProject\StomatologyRobot\res\Handpiece.stl)");
  toolReader->Update();
  auto toolMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  toolMapper->SetInputData(toolReader->GetOutput());
  auto toolActor = vtkSmartPointer<vtkActor>::New();
  toolActor->SetMapper(toolMapper);
  // toolActor->GetProperty()->SetRepresentationToWireframe();
  renderer->AddActor(toolActor);
  auto sequencePass = vtkSmartPointer<vtkSequencePass>::New();
  auto renderPassCollections = vtkSmartPointer<vtkRenderPassCollection>::New();
  sequencePass->SetPasses(renderPassCollections);
  // auto colorChangePass = vtkSmartPointer<ColorChangePass>::New();
  auto opaqueRenderPass = vtkSmartPointer<OpaqueDepthRenderPass>::New();
  renderPassCollections->AddItem(opaqueRenderPass);
  auto actorCollections = vtkSmartPointer<vtkActorCollection>::New();
  actorCollections->AddItem(workpieceActor);
  opaqueRenderPass->SetOpaqueActors(actorCollections);
  renderer->SetPass(opaqueRenderPass);

  renderWindowInteractor->Start();
}
