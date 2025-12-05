#include "RenderWindow.h"
#include "DicomReader.hpp"
#include "DicomSeries.h"
#include "ImageResliceFilter.h"
#include "Plane.h"
#include "Volume.h"
#include <glm/glm.hpp>
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
    mResliceFilter->SetInputData(mImageData);
    mResliceFilter->SetPlaneLocalToWorldTransform(mPlaneIndexToWorldTransform);
    auto outputImageData = mResliceFilter->GetOutput();
  }

public:
  vtkSmartPointer<vtkPlaneSource> mPlaneSource;
  vtkSmartPointer<vtkTexture> mTexture;
  vtkSmartPointer<ImageResliceFilter> mResliceFilter;
  vtkSmartPointer<vtkTransform> mPlaneIndexToWorldTransform;
  vtkSmartPointer<vtkImageData> mImageData;
  int mCount;

protected:
  RenderCallBack()
    : mCount(0)
  {
  }
  ~RenderCallBack() override {}
};

RenderWindow::RenderWindow(QWidget* parent)
  : QVTKOpenGLNativeWidget(parent)
{
  mImageResliceFilter = vtkSmartPointer<ImageResliceFilter>::New();

  using ReaderType = DicomReadReader<itk::Image<float, 3>>;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetDicomDirectory("D:/Workspace/Data/case2");
  reader->GenerateData();

  auto dicomSeries = *(reader->begin());
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
  planeIndexToWorldMatrix->SetElement(2, 3, origin[2] + 20.0);

  mPlaneIndexToWorldTransform->SetMatrix(planeIndexToWorldMatrix);
  mImageResliceFilter->SetPlaneLocalToWorldTransform(mPlaneIndexToWorldTransform);
  mImageResliceFilter->SetInputData(imageData);
  mImageResliceFilter->Update();


  mPlaneSource = vtkSmartPointer<vtkPlaneSource>::New();
  mPlaneSource->SetOrigin(0, 0, 0);
  mPlaneSource->SetPoint1(1, 0, 0);
  mPlaneSource->SetPoint2(0, 1, 0);
  mPlaneActor = vtkSmartPointer<vtkActor>::New();
  vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  mapper->SetInputConnection(mPlaneSource->GetOutputPort());
  mPlaneActor->SetMapper(mapper);
  vtkSmartPointer<vtkTexture> texture = vtkSmartPointer<vtkTexture>::New();
  texture->SetInputData(mImageResliceFilter->GetOutput());
  texture->SetColorMode(VTK_COLOR_MODE_MAP_SCALARS);
  mPlaneActor->SetTexture(texture);
  vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();

  renderer->AddActor(mPlaneActor);
  GetRenderWindow()->AddRenderer(renderer);

//   vtkSmartPointer<RenderCallBack> renderCallBack = vtkSmartPointer<RenderCallBack>::New();
//   renderCallBack->mTexture = texture;
//   renderCallBack->mPlaneSource = mPlaneSource;
//   renderCallBack->mResliceFilter = mImageResliceFilter;
//   renderCallBack->mPlaneIndexToWorldTransform = mPlaneIndexToWorldTransform;

//   GetInteractor()->CreateRepeatingTimer(100);
//   GetInteractor()->AddObserver(vtkCommand::TimerEvent, renderCallBack);
}

RenderWindow::~RenderWindow() {}
