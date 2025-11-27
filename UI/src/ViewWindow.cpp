#include "ViewWindow.h"
#include <spdlog/spdlog.h>
#include <vtkFloatArray.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkSmartPointer.h>

struct ViewWindow::Impl
{
  vtkSmartPointer<vtkRenderer> m_Renderer;
  int m_ViewId;
  Impl(int viewId)
    : m_ViewId(viewId)
  {
    m_Renderer = vtkSmartPointer<vtkRenderer>::New();
    m_Renderer->SetLayer(0);
    m_ViewId = -1;
  }
};

ViewWindow::ViewWindow(int viewId, QObject* parent)
  : QObject(parent)
{
  m_Impl = std::make_unique<Impl>(viewId);
}

ViewWindow::~ViewWindow() {}

void ViewWindow::setViewSize(vtkFloatArray* viewSize)
{
  if (!viewSize)
  {
    SPDLOG_DEBUG("view size is nullptr");
    return;
  }
  if (viewSize->GetNumberOfComponents() != 4)
  {
    SPDLOG_DEBUG("view size type is wrong");
    return;
  }

  m_Impl->m_Renderer->SetViewport(viewSize->GetComponent(0, 0), viewSize->GetComponent(0, 1),
    viewSize->GetComponent(0, 2), viewSize->GetComponent(0, 3));
}

void ViewWindow::setBackground(vtkFloatArray* background)
{
  if (!background)
  {
    SPDLOG_DEBUG("background is nullptr");
    return;
  }
  if (background->GetNumberOfComponents() != 4)
  {
    SPDLOG_DEBUG("background components error");
    return;
  }

  m_Impl->m_Renderer->SetBackground(
    background->GetComponent(0, 0), background->GetComponent(0, 1), background->GetComponent(0, 2));
  m_Impl->m_Renderer->SetBackgroundAlpha(background->GetComponent(0, 3));
}

void ViewWindow::addToWindow(vtkRenderWindow* window)
{
  if (!window)
  {
    return;
  }

  window->AddRenderer(m_Impl->m_Renderer);
}

int ViewWindow::getViewId() const
{
  return m_Impl->m_ViewId;
}
