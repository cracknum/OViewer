#include <iostream>
#include <vtkImageData.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkNIFTIImageReader.h>
#include <vtkOpenGLRenderWindow.h>
#include <vtkOpenGLState.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkSTLReader.h>
#include <vtkSmartPointer.h>
#include <vtkTextureObject.h>
#include <vtkTextureUnitManager.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkProperty.h>
#include "ColorChangePass.h"

#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080
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

  reader->SetFileName(R"(D:\Workspace\gitProject\build\bin\Release\data\1\coarsePredict\17\mask_tooth_crop.nii.gz)");
  reader->Update();
  auto workpieceData = reader->GetOutput();
  int dimensions[3]{};
  double spacing[3]{};
  workpieceData->GetDimensions(dimensions);
  workpieceData->GetSpacing(spacing);
  
  auto toolReader = vtkSmartPointer<vtkSTLReader>::New();
  toolReader->SetFileName(R"(D:\Workspace\gitProject\StomatologyRobot\res\Handpiece.stl)");
  toolReader->Update();
  auto toolMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  toolMapper->SetInputData(toolReader->GetOutput());
  auto toolActor = vtkSmartPointer<vtkActor>::New();
  toolActor->SetMapper(toolMapper);
  toolActor->GetProperty()->SetRepresentationToWireframe();
  renderer->AddActor(toolActor);
  

  auto colorChangePass = vtkSmartPointer<ColorChangePass>::New();
  renderer->SetPass(colorChangePass);

  renderWindowInteractor->Start();
}