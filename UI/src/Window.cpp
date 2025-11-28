#include "Window.h"
#include "Style.h"
#include "ViewWindow.h"
#include <spdlog/spdlog.h>
#include <vtkActor2D.h>
#include <vtkCoordinate.h>
#include <vtkFloatArray.h>
#include <vtkLineSource.h>
#include <vtkOpenGLPolyDataMapper2D.h>
#include <vtkPointData.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper2D.h>
#include <vtkProgrammableFilter.h>
#include <vtkProperty2D.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkShaderProperty.h>
#include <vtkUnsignedCharArray.h>
#include "DicomSeries.h"
#include <itkImageToVTKImageFilter.h>
#include <vtkImageData.h>
struct Window::Impl
{
  std::vector<ViewWindow*> m_ViewWindows;
  vtkSmartPointer<vtkRenderer> m_OverlayRenderer;
  vtkSmartPointer<vtkImageData> m_ImageData;
  Impl()
  {
    m_OverlayRenderer = vtkSmartPointer<vtkRenderer>::New();
    m_OverlayRenderer->SetLayer(1);
  }
};

inline Window::Window(QWidget* parent)
  : QVTKOpenGLNativeWidget(parent)
{
  renderWindow()->SetNumberOfLayers(2);
  m_Impl = std::make_unique<Impl>();
  renderWindow()->AddRenderer(m_Impl->m_OverlayRenderer);
}

Window::~Window() = default;

void Window::addViewWindow(int viewId, vtkFloatArray* windowRange, vtkFloatArray* background)
{
  auto viewWindow = new ViewWindow(viewId, this);

  viewWindow->setViewSize(windowRange);
  viewWindow->setBackground(background);

  viewWindow->addToWindow(renderWindow());

  m_Impl->m_ViewWindows.push_back(viewWindow);

  SPDLOG_INFO("add view window {}", viewId);
}

void Window::addSplitLine(vtkFloatArray* colorArray, vtkFloatArray* linePoints)
{
  auto actor = createLineActor(colorArray, linePoints);
  m_Impl->m_OverlayRenderer->AddActor(actor);
  SPDLOG_INFO("add split line");
}

ViewWindow* Window::getViewWindow(int viewId) const
{
  auto it = std::find_if(m_Impl->m_ViewWindows.begin(), m_Impl->m_ViewWindows.end(),
    [viewId](ViewWindow* window) { return window->getViewId() == viewId; });
  if (it != m_Impl->m_ViewWindows.end())
  {
    return *it;
  }
  return nullptr;
}

vtkSmartPointer<vtkActor2D> Window::createLineActor(
  vtkFloatArray* colorArray, vtkFloatArray* linePoints)
{
  if (!colorArray || !linePoints)
  {
    SPDLOG_DEBUG("colorArray is nullptr: {}, linePoints is nullptr: {}", colorArray == nullptr,
      linePoints == nullptr);
    return nullptr;
  }

  if (linePoints->GetNumberOfTuples() < 2)
  {
    SPDLOG_DEBUG("number of linePoints", linePoints->GetNumberOfTuples());
    return nullptr;
  }

  if (colorArray->GetNumberOfComponents() != 4 || colorArray->GetNumberOfTuples() != 2)
  {
    SPDLOG_DEBUG("colorArray components must be 4 and it's number of tuples must be 2");
    return nullptr;
  }

  vtkSmartPointer<vtkLineSource> line = vtkSmartPointer<vtkLineSource>::New();
  line->SetPoint1(linePoints->GetTuple3(0));
  line->SetPoint2(linePoints->GetTuple3(1));
  line->Update();
  auto polyData = line->GetOutput();
  vtkNew<vtkUnsignedCharArray> charColor;
  charColor->SetNumberOfComponents(4);
  charColor->InsertNextTuple4(static_cast<unsigned char>(colorArray->GetComponent(0, 0) * 255),
    static_cast<unsigned char>(colorArray->GetComponent(0, 1) * 255),
    static_cast<unsigned char>(colorArray->GetComponent(0, 2) * 255),
    static_cast<unsigned char>(colorArray->GetComponent(0, 3) * 255));
  charColor->InsertNextTuple4(static_cast<unsigned char>(colorArray->GetComponent(1, 0) * 255),
    static_cast<unsigned char>(colorArray->GetComponent(1, 1) * 255),
    static_cast<unsigned char>(colorArray->GetComponent(1, 2) * 255),
    static_cast<unsigned char>(colorArray->GetComponent(1, 3) * 255));

  SPDLOG_DEBUG("color1: {} {} {} {}", charColor->GetComponent(0, 0), charColor->GetComponent(0, 1),
    charColor->GetComponent(0, 2), charColor->GetComponent(0, 3));
  SPDLOG_DEBUG("color2: {} {} {} {}", charColor->GetComponent(1, 0), charColor->GetComponent(1, 1),
    charColor->GetComponent(1, 2), charColor->GetComponent(1, 3));
  polyData->GetPointData()->SetScalars(charColor);
  vtkNew<vtkCoordinate> coord;
  coord->SetCoordinateSystemToNormalizedViewport();
  vtkSmartPointer<vtkPolyDataMapper2D> mapper = vtkSmartPointer<vtkPolyDataMapper2D>::New();
  mapper->SetInputData(polyData);
  mapper->SetTransformCoordinate(coord);
  mapper->SetScalarVisibility(1);
  vtkSmartPointer<vtkActor2D> actor = vtkSmartPointer<vtkActor2D>::New();
  actor->SetMapper(mapper);
  actor->GetProperty()->SetLineWidth(5.0f);

  return actor;
}

void Window::openImage(DicomSeries* dicom)
{
	auto imageInfo = dicom->GetImageInfo();
	m_Impl->m_ImageData = imageInfo->GetVtkVolume();
}