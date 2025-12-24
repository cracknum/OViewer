#include "MPRSliceRepresentation.h"
#include "MPRSliceWidget.h"
#include <gtest/gtest.h>
#include <vtkAutoInit.h>
#include <vtkCommand.h>
#include <vtkImageData.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkInteractorStyleUser.h>
#include <vtkNIFTIImageReader.h>
#include <vtkObjectFactory.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkSmartPointer.h>
#include <vtkWidgetRepresentation.h>
VTK_MODULE_INIT(vtkRenderingOpenGL2);
VTK_MODULE_INIT(vtkInteractionStyle); // 👈 关键！消除警告
VTK_MODULE_INIT(vtkRenderingFreeType);

class WheelCommand : public vtkCommand
{
public:
  vtkTypeMacro(WheelCommand, vtkCommand);
  static WheelCommand* New();
  void Execute(vtkObject* caller, unsigned long eventId, void* callData) override
  {
    auto interactor = vtkRenderWindowInteractor::SafeDownCast(caller);
    interactor->InvokeEvent(vtkCommand::MouseWheelForwardEvent, nullptr);
  }
};

vtkStandardNewMacro(WheelCommand);
int main()
{
  auto niftiReader = vtkSmartPointer<vtkNIFTIImageReader>::New();
  niftiReader->SetFileName(R"(D:\Workspace\Data\case2.nii.gz)");
  niftiReader->Update();
  auto mImageData = niftiReader->GetOutput();
  auto mLevel = 1024;
  auto mWindow = 4096;

  auto mprSliceWidget = vtkSmartPointer<MPRSliceWidget>::New();
  mprSliceWidget->setImageData(mImageData);
  mprSliceWidget->setWindow(mWindow);
  mprSliceWidget->setLevel(mLevel);
  mprSliceWidget->setSliceType(StandardPlane::Axial);

  auto renderer = vtkSmartPointer<vtkRenderer>::New();
  auto rep = MPRSliceRepresentation::SafeDownCast(mprSliceWidget->GetRepresentation());
  renderer->AddViewProp(rep);

  auto renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
  auto interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
  auto interactorStyle = vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();
  renderWindow->AddRenderer(renderer);
  renderWindow->SetInteractor(interactor);
  interactor->SetInteractorStyle(interactorStyle);
  mprSliceWidget->SetInteractor(interactor);
  mprSliceWidget->EnabledOn();
  interactor->Initialize();
  interactor->CreateRepeatingTimer(100);
  auto wheelCommand = vtkSmartPointer<WheelCommand>::New();
  interactor->AddObserver(vtkCommand::TimerEvent, wheelCommand);
  interactor->Start();

  return 0;
}
