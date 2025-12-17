#include "RenderWindow.h"
#include "DicomReader.hpp"
#include "DicomSeries.h"
#include <spdlog/spdlog.h>
#include <vtkActor.h>
#include <vtkCommand.h>
#include <vtkImageData.h>
#include <vtkMatrix4x4.h>
#include <vtkOpenGLActor.h>
#include <vtkPlaneSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkTexture.h>
#include <vtkTextureObject.h>
#include <vtkTransform.h>

class RenderCallBack : public vtkCommand
{
public:
  static RenderCallBack* New();
  vtkTypeMacro(RenderCallBack, vtkCommand);
  void Execute(vtkObject* caller, unsigned long eventId, void* callData) override
  {
    mPlaneIndexToWorldTransform->Translate(0, 0, 0.1);
    mInteractor->Render();
  }

public:
  vtkSmartPointer<vtkPlaneSource> mPlaneSource;
  vtkSmartPointer<vtkTransform> mPlaneIndexToWorldTransform;
  vtkSmartPointer<vtkImageData> mImageData;
  vtkSmartPointer<vtkRenderWindowInteractor> mInteractor;
  int mCount;

protected:
  RenderCallBack()
    : mCount(0)
  {
  }
  ~RenderCallBack() override {}
};

vtkStandardNewMacro(RenderCallBack);

RenderWindow::RenderWindow(QWidget* parent)
  : QVTKOpenGLNativeWidget(parent)
{
  using ReaderType = DicomReadReader<itk::Image<float, 3>>;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetDicomDirectory("F:/Workspace/Data/Covid Scans");
  reader->GenerateData();

  auto dicomSeries = *(reader->begin() + 1);
  auto imageData = dicomSeries->GetImageInfo()->GetVtkVolume();

  double* origin = imageData->GetOrigin();
  double* spacing = imageData->GetSpacing();
  int* dimensions = imageData->GetDimensions();

  mPlaneIndexToWorldTransform = vtkSmartPointer<vtkTransform>::New();
  vtkSmartPointer<vtkMatrix4x4> planeIndexToWorldMatrix = vtkSmartPointer<vtkMatrix4x4>::New();
  planeIndexToWorldMatrix->Identity();
  planeIndexToWorldMatrix->SetElement(0, 0, spacing[0]);
  planeIndexToWorldMatrix->SetElement(1, 1, spacing[1]);
  planeIndexToWorldMatrix->SetElement(2, 2, spacing[2]);
  planeIndexToWorldMatrix->SetElement(0, 3, origin[0]);
  planeIndexToWorldMatrix->SetElement(1, 3, origin[1]);
  planeIndexToWorldMatrix->SetElement(2, 3, origin[2] + 1);

  mPlaneIndexToWorldTransform->SetMatrix(planeIndexToWorldMatrix);

  mPlaneSource = vtkSmartPointer<vtkPlaneSource>::New();
  mPlaneSource->SetOrigin(0, 0, 0);
  mPlaneSource->SetPoint1(1, 0, 0);
  mPlaneSource->SetPoint2(0, -1, 0);
  mPlaneActor = vtkSmartPointer<vtkActor>::New();
  vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  mapper->SetInputConnection(mPlaneSource->GetOutputPort());
  mPlaneActor->SetMapper(mapper);
  vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();

  renderer->AddActor(mPlaneActor);
  renderWindow()->AddRenderer(renderer);

  vtkSmartPointer<RenderCallBack> renderCallBack = vtkSmartPointer<RenderCallBack>::New();
  renderCallBack->mPlaneSource = mPlaneSource;
  renderCallBack->mPlaneIndexToWorldTransform = mPlaneIndexToWorldTransform;
  renderCallBack->mInteractor = interactor();

  interactor()->CreateRepeatingTimer(100);
  interactor()->AddObserver(vtkCommand::TimerEvent, renderCallBack);
}

RenderWindow::~RenderWindow() {}
