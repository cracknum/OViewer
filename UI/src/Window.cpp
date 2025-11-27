#include "Style.h"
#include "ViewWindow.h"
#include "Window.h"
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
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkShaderProperty.h>
#include <vtkUnsignedCharArray.h>

struct Window::Impl
{
  vtkSmartPointer<vtkFloatArray> m_ColorArray;
  vtkSmartPointer<vtkActor2D> m_HorizontalLineActor;
  vtkSmartPointer<vtkActor2D> m_VerticalLineActor;
  std::vector<ViewWindow*> m_ViewWindows;
  vtkSmartPointer<vtkRenderer> m_OverlayRenderer;
  Impl()
  {
    m_ColorArray = vtkSmartPointer<vtkFloatArray>::New();
    m_OverlayRenderer = vtkSmartPointer<vtkRenderer>::New();
    m_OverlayRenderer->SetLayer(1);
    m_ColorArray->SetName("SplitLineColors");
    m_ColorArray->SetNumberOfComponents(3);
    m_ColorArray->SetNumberOfTuples(2);

    m_ColorArray->SetTuple3(0, 0.0f, 1.0f, 0.0f);
    m_ColorArray->SetTuple3(1, 1.0f, 0.0f, 0.0f);
  }
};

inline Window::Window(QWidget* parent)
  : QVTKOpenGLNativeWidget(parent)
{
  GetRenderWindow()->SetNumberOfLayers(2);
  m_Impl = std::make_unique<Impl>();
  GetRenderWindow()->AddRenderer(m_Impl->m_OverlayRenderer);

  initViewWindows();
  initSplitLine();
}
inline void Window::initSplitLine()
{
  vtkNew<vtkFloatArray> horizontalLinePointsArray;
  vtkNew<vtkFloatArray> verticalLinePointsArray;
  horizontalLinePointsArray->SetNumberOfComponents(3);
  horizontalLinePointsArray->InsertNextTuple3(0.0f, 0.5f, 0.0f);
  horizontalLinePointsArray->InsertNextTuple3(1.0f, 0.5f, 0.0f);

  verticalLinePointsArray->SetNumberOfComponents(3);
  verticalLinePointsArray->InsertNextTuple3(0.5f, 0.0f, 0.0f);
  verticalLinePointsArray->InsertNextTuple3(0.5f, 1.0f, 0.0f);
  addSplitLine(m_Impl->m_ColorArray, horizontalLinePointsArray);
  addSplitLine(m_Impl->m_ColorArray, verticalLinePointsArray);
}
void Window::initViewWindows()
{
  vtkNew<vtkFloatArray> viewWindow1Size, viewWindow2Size, viewWindow3Size, viewWindow4Size;
  vtkNew<vtkFloatArray> background1, background2;
  viewWindow1Size->SetNumberOfComponents(4);
  viewWindow2Size->SetNumberOfComponents(4);
  viewWindow3Size->SetNumberOfComponents(4);
  viewWindow4Size->SetNumberOfComponents(4);

  viewWindow1Size->InsertNextTuple4(0.0f, 0.0f, 0.5f, 0.5f);
  viewWindow2Size->InsertNextTuple4(0.5f, 0.0f, 1.0f, 0.5f);
  viewWindow3Size->InsertNextTuple4(0.0f, 0.5f, 0.5f, 1.0f);
  viewWindow4Size->InsertNextTuple4(0.5f, 0.5f, 1.0f, 1.0f);
  background1->SetNumberOfComponents(4);
  background1->InsertNextTuple4(0.0f, 0.0f, 0.0f, 1.0f);
  background2->SetNumberOfComponents(4);
  background2->InsertNextTuple4(0.0f, 0.0f, 0.0f, 1.0f);

  addViewWindow(0, viewWindow1Size, background1);
  addViewWindow(1, viewWindow2Size, background2);
  addViewWindow(2, viewWindow3Size, background2);
  addViewWindow(3, viewWindow4Size, background1);
}
Window::~Window() = default;

void Window::addViewWindow(int viewId, vtkFloatArray* windowRange, vtkFloatArray* background)
{
  auto viewWindow = new ViewWindow(viewId, this);

  viewWindow->setViewSize(windowRange);
  viewWindow->setBackground(background);

  viewWindow->addToWindow(GetRenderWindow());

  m_Impl->m_ViewWindows.push_back(viewWindow);
}

void Window::addSplitLine(vtkFloatArray* colorArray, vtkFloatArray* linePoints)
{
  auto actor = createLineActor(colorArray, linePoints);
  m_Impl->m_OverlayRenderer->AddActor(actor);
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

  vtkSmartPointer<vtkLineSource> line = vtkSmartPointer<vtkLineSource>::New();
  line->SetPoint1(linePoints->GetTuple3(0));
  line->SetPoint2(linePoints->GetTuple3(1));
  line->Update();
  auto polyData = line->GetOutput();
  polyData->GetPointData()->SetScalars(m_Impl->m_ColorArray);
  vtkNew<vtkCoordinate> coord;
  coord->SetCoordinateSystemToNormalizedViewport();
  vtkSmartPointer<vtkPolyDataMapper2D> mapper = vtkSmartPointer<vtkPolyDataMapper2D>::New();
  mapper->SetInputData(polyData);
  mapper->SetTransformCoordinate(coord);
  vtkSmartPointer<vtkActor2D> actor = vtkSmartPointer<vtkActor2D>::New();
  actor->SetMapper(mapper);
  actor->GetProperty()->SetLineWidth(5.0f);

  return actor;
}
