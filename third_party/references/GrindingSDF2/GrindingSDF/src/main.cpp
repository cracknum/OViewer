#include <vtkActor.h>
#include <vtkCubeSource.h>
#include <vtkGrindDef.h>
#include <vtkGrindInteractor.h>
#include <vtkInformation.h>
#include <vtkMatrix4x4.h>
#include <vtkOpenGLPolyDataMapper.h>
#include <vtkOpenGLRendererEx.h>
#include <vtkOpenGLRenderWindow.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkSmartPointer.h>
#include <vtkSphereSource.h>

#include <iostream>

#include "vtkAutoInit.h"
VTK_MODULE_INIT(vtkRenderingOpenGL2);
VTK_MODULE_INIT(vtkInteractionStyle);
static float BONE_CLR[3] = {1.f, 1.f, 1.f};

int main() {
  auto renderer = vtkSmartPointer<vtkOpenGLRendererEx>::New();
  auto renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
  auto renderWindowInteractor =
      vtkSmartPointer<vtkRenderWindowInteractor>::New();
  auto cubeSource = vtkSmartPointer<vtkSphereSource>::New();
  auto cubeMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  auto cubeActor = vtkSmartPointer<vtkActor>::New();
  cubeSource->SetRadius(10);
  cubeSource->SetCenter(0, 0, 0);
  cubeSource->Update();
  cubeMapper->SetScalarVisibility(false);


  cubeMapper->SetInputData(cubeSource->GetOutput());
  cubeActor->SetMapper(cubeMapper);
  cubeActor->GetProperty()->SetColor(BONE_CLR[0], BONE_CLR[1], BONE_CLR[2]);
  cubeActor->GetProperty()->SetAmbient(0.4f);
  cubeActor->GetProperty()->SetDiffuse(0.5f);
  cubeActor->GetProperty()->SetSpecular(0);

  renderer->AddActor(cubeActor);

  auto grindInteractor = renderer->GetGrindInteractor();
  grindInteractor->AllocateToolTrack(1);
  grindInteractor->ActiveToolTrack(0);
  grindInteractor->SetGrindType(vtkGrindDef::GT_Geom);
  grindInteractor->SetGrindBaseColor(BONE_CLR);
  float expand_default_clr[3] = {1.f, 0.f, 0.f};
  grindInteractor->SetGrindExpandDefaultColor(expand_default_clr);
  grindInteractor->AddGrindExpand(1.0, BONE_CLR);
  grindInteractor->SetGrindResectColor(BONE_CLR);
  auto mat = vtkSmartPointer<vtkMatrix4x4>::New();
  mat->Identity();
  mat->SetElement(0, 3, 5);
  mat->SetElement(1, 3, 5);
  mat->SetElement(2, 3, 5);
  grindInteractor->SetSphereGrindResect(5, mat);
  grindInteractor->SetSphereTool(5);
  grindInteractor->ToggleToolTrack();

  vtkInformation *info = cubeActor->GetPropertyKeys();
  if (!info) {
    info = vtkInformation::New();
    cubeActor->SetPropertyKeys(info);
    info->FastDelete();
  }
  info->Set(vtkGrindInteractor::GrindRequest(), "");
  cubeActor->SetVisibility(1);
  renderWindow->AddRenderer(renderer);
  renderWindowInteractor->SetRenderWindow(renderWindow);
  renderWindow->Render();

  renderWindowInteractor->Start();
}
