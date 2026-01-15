#include "HollowCubeOrientationMarkerBuilder.h"
#include "HollowCubeOrientationMarkerWidget.h"
#include <vtkActor.h>
#include <vtkAssembly.h>
#include <vtkCubeSource.h>
#include <vtkFollower.h>
#include <vtkOBBTree.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRenderer.h>
#include <vtkVectorText.h>

HollowCubeOrientationMarker::HollowCubeOrientationMarker(vtkRenderWindowInteractor* interactor)
  : CubeOrientationMarkerBuilder(interactor)
{
}

HollowCubeOrientationMarker::~HollowCubeOrientationMarker() {}

vtkSmartPointer<HollowCubeOrientationMarkerWidget> HollowCubeOrientationMarker::Build()
{
    vtkNew<vtkCubeSource> cubeSource;
    cubeSource->SetXLength(1);
    cubeSource->SetYLength(1);
    cubeSource->SetZLength(1);
    cubeSource->SetCenter(0, 0, 0);
    cubeSource->Update();

    vtkNew<vtkPolyDataMapper> cubeMapper;
    cubeMapper->SetInputConnection(cubeSource->GetOutputPort());
    vtkNew<vtkActor> cubeActor;
    cubeActor->SetMapper(cubeMapper);
    cubeActor->GetProperty()->SetColor(0.0, 0.0, 0.0);
    cubeActor->GetProperty()->SetLineWidth(2.0);
    cubeActor->GetProperty()->RenderLinesAsTubesOn();

    vtkNew<vtkCubeSource> cubeSourceBounds;
    cubeSourceBounds->SetXLength(1);
    cubeSourceBounds->SetYLength(1);
    cubeSourceBounds->SetZLength(1);
    cubeSourceBounds->SetCenter(0, 0, 0);
    cubeSourceBounds->Update();

    vtkNew<vtkPolyDataMapper> cubeMapperBounds;
    cubeMapperBounds->SetInputConnection(cubeSourceBounds->GetOutputPort());
    vtkNew<vtkActor> cubeActorBounds;
    cubeActorBounds->SetMapper(cubeMapperBounds);
    cubeActorBounds->GetProperty()->SetColor(1.0, 1.0, 1.0);
    cubeActorBounds->GetProperty()->SetLineWidth(2.0);
    cubeActorBounds->GetProperty()->RenderLinesAsTubesOn();
    cubeActorBounds->GetProperty()->SetRepresentationToWireframe();

    vtkNew<vtkAssembly> assembly;
    AddVectorTexts(assembly);

    vtkNew<vtkAssembly> orientationMarkerAssembly;
    orientationMarkerAssembly->AddPart(cubeActor);
    orientationMarkerAssembly->AddPart(cubeActorBounds);

    auto orientationMarker = vtkSmartPointer<HollowCubeOrientationMarkerWidget>::New();
    orientationMarker->SetOrientationMarker(orientationMarkerAssembly);
    orientationMarker->SetTextActors(assembly);
    orientationMarker->SetInteractor(mInteractor);
    orientationMarker->EnabledOn();
    orientationMarker->SetInteractive(false);

    vtkNew<vtkOBBTree> obbTree;
    obbTree->SetDataSet(cubeSource->GetOutput());
    obbTree->BuildLocator();
    orientationMarker->SetCubeOBBTree(obbTree);

    return orientationMarker;
}

void HollowCubeOrientationMarker::AddVectorText(
  vtkAssembly* assembly, const char* text, double* origin, double scale)
{
    vtkNew<vtkVectorText> textSource;
    textSource->SetText(text);
    vtkNew<vtkPolyDataMapper> textMapper;
    textMapper->SetInputConnection(textSource->GetOutputPort());

    vtkNew<vtkFollower> textActor;
    textActor->SetMapper(textMapper);
    textActor->SetOrigin(origin);
    textActor->SetScale(scale);

    assembly->AddPart(textActor);
}

void HollowCubeOrientationMarker::AddVectorTexts(vtkAssembly* assembly)
{
    double rightOrigin[3] = { -0.7, 0, 0 };
    double leftOrigin[3] = { 0.7, 0, 0 };
    double posteriorOrigin[3] = { 0, 0.7, 0 };
    double anteriorOrigin[3] = { 0, -0.7, 0 };
    double headOrigin[3] = { 0, 0, 0.7 };
    double footOrigin[3] = { 0, 0, -0.7 };

    AddVectorText(assembly, "L", leftOrigin, 0.1);
    AddVectorText(assembly, "R", rightOrigin, 0.1);
    AddVectorText(assembly, "H", headOrigin, 0.1);
    AddVectorText(assembly, "F", footOrigin, 0.1);
    AddVectorText(assembly, "A", anteriorOrigin, 0.1);
    AddVectorText(assembly, "P", posteriorOrigin, 0.1);
}
