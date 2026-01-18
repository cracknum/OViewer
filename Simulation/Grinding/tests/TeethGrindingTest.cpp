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
#include <vtkPolyDataNormals.h>
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
#include <vtkXMLPolyDataReader.h>

#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080

vtkSmartPointer<vtkActor> reconstructWorkpiece(vtkImageData* imageData)
{
  auto flyingEdgesAlgo = vtkSmartPointer<vtkFlyingEdges3D>::New();
  flyingEdgesAlgo->SetInputData(imageData);
  flyingEdgesAlgo->SetValue(0, 17);
  auto normals = vtkSmartPointer<vtkPolyDataNormals>::New();
  normals->SetInputConnection(flyingEdgesAlgo->GetOutputPort());
  normals->SetComputePointNormals(true); // 计算点法线（用于平滑着色）
  normals->SetComputeCellNormals(false); // 通常不需要面法线
  normals->SetSplitting(false);          // 避免在尖锐边缘分裂（可选）
  auto smoothPolyDataFilter = vtkSmartPointer<vtkWindowedSincPolyDataFilter>::New();
  smoothPolyDataFilter->SetInputConnection(normals->GetOutputPort());
  smoothPolyDataFilter->SetNumberOfIterations(15);
  smoothPolyDataFilter->BoundarySmoothingOff();
  smoothPolyDataFilter->FeatureEdgeSmoothingOff();
  smoothPolyDataFilter->SetFeatureAngle(120.0);
  smoothPolyDataFilter->SetPassBand(.001);
  smoothPolyDataFilter->NonManifoldSmoothingOn();
  smoothPolyDataFilter->NormalizeCoordinatesOn();

  auto mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  mapper->SetInputConnection(smoothPolyDataFilter->GetOutputPort());
  mapper->SetScalarVisibility(0);
  auto actor = vtkSmartPointer<vtkActor>::New();
  actor->SetMapper(mapper);
  actor->GetProperty()->SetColor(1.0, 0.0, 0.0);

  return actor;
}
int main()
{
  auto reader = vtkXMLPolyDataReader::New();
  auto renderer = vtkSmartPointer<vtkRenderer>::New();
  auto renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
  auto renderWindowInteractor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
  auto renderWindowStyle = vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();
  renderWindow->AddRenderer(renderer);
  renderWindow->SetSize(1920, 1080);
  renderWindow->SetMultiSamples(0);
  renderWindowInteractor->SetRenderWindow(renderWindow);
  renderWindowInteractor->SetInteractorStyle(renderWindowStyle);
  renderWindowInteractor->Initialize();

  std::string dcmPath = std::string(TESTS_SOURCE_DIR) + "/mask_tooth_crop_masked_intersect.vtp";
  reader->SetFileName(dcmPath.c_str());
  reader->Update();
  auto workpieceData = reader->GetOutput();
  double bounds[6]{};
  workpieceData->GetBounds(bounds);
  double origin[3]{ bounds[0], bounds[2], bounds[4] };
  double spacing[3]{ 0.1, 0.1, 0.1 };

  int dimensions[3]{ (bounds[1] - bounds[0]) / spacing[0], (bounds[3] - bounds[2]) / spacing[1],
    (bounds[5] - bounds[4]) / spacing[2] };

  SPDLOG_INFO("workpiece origin: {} {} {}", origin[0], origin[1], origin[2]);
  SPDLOG_INFO("workpiece spacing: {} {} {}", spacing[0], spacing[1], spacing[2]);
  SPDLOG_INFO("workpiece dimensions: {} {} {}", dimensions[0], dimensions[1], dimensions[2]);

  auto workpieceMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  workpieceMapper->SetScalarVisibility(0);
  workpieceMapper->SetInputData(workpieceData);
  auto workpieceActor = vtkSmartPointer<vtkActor>::New();
  workpieceActor->SetMapper(workpieceMapper);

  renderer->AddActor(workpieceActor);
  auto information = vtkInformation::New();
  workpieceActor->SetPropertyKeys(information);
  information->FastDelete();
  information->Set(GrindingRenderPass::DimensionsInfo(), dimensions, 3);
  information->Set(GrindingRenderPass::OriginInfo(), origin, 3);
  information->Set(GrindingRenderPass::SpacingInfo(), spacing, 3);

  auto toolReader = vtkSmartPointer<vtkSTLReader>::New();
  std::string toolPath = std::string(TESTS_SOURCE_DIR) + "/Handpiece.stl";
  toolReader->SetFileName(toolPath.c_str());
  toolReader->Update();
  auto toolMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  toolMapper->SetInputData(toolReader->GetOutput());
  auto toolActor = vtkSmartPointer<vtkActor>::New();
  toolActor->SetMapper(toolMapper);
  toolActor->GetProperty()->SetRepresentationToWireframe();
  renderer->AddActor(toolActor);
  auto grindingRenderPass = vtkSmartPointer<GrindingRenderPass>::New();
  auto actorCollections = vtkSmartPointer<vtkActorCollection>::New();
  actorCollections->AddItem(workpieceActor);
  grindingRenderPass->SetWorkpieceActors(actorCollections);
  renderer->SetPass(grindingRenderPass);

  renderWindowInteractor->Start();
}
