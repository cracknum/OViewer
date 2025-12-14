#include "OpenGLViewerWidget.h"
#include "DcmEventData.h"
#include "DicomSeries.h"
#include "EventId.h"
#include "MouseEvent.h"
#include "MouseEventData.h"
#include <imgui.h>
#include <spdlog/spdlog.h>
struct OpenGLViewerWidget::Private
{
  std::shared_ptr<FrameBuffer> mFrameBuffer;
  itk::SmartPointer<DicomSeries> mDicomSeries;
};
OpenGLViewerWidget::OpenGLViewerWidget(const char* widgetNamae, int widgetFlags)
  : Widget(widgetNamae, widgetFlags)
{
  mPrivate = std::make_unique<Private>();
}

OpenGLViewerWidget::~OpenGLViewerWidget() = default;

bool OpenGLViewerWidget::render()
{
  ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);            // 无边框
  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f)); // 无边界
  ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);              // 无圆角

  if (ImGui::Begin(mWidgetName.c_str(), nullptr, mWidgetFlags))
  {
    const ImVec2 canvasSize = ImGui::GetContentRegionAvail();

    if (!mPrivate->mFrameBuffer)
    {
      mPrivate->mFrameBuffer = std::make_unique<FrameBuffer>(canvasSize.x, canvasSize.y);
      SPDLOG_DEBUG("framebuffer size: {} {}", canvasSize.x, canvasSize.y);
    }
    else if (mPrivate->mFrameBuffer->textureWidth() - canvasSize.x != 0 ||
      mPrivate->mFrameBuffer->textureHeight() - canvasSize.y != 0)
    {
      mPrivate->mFrameBuffer->updateBufferSize(canvasSize.x, canvasSize.y);
      SPDLOG_DEBUG("framebuffer size: {} {}", canvasSize.x, canvasSize.y);
    }

    if (canvasSize.x <= 0 || canvasSize.y <= 0)
    {
      if (!mPrivate->mFrameBuffer)
      {
        mPrivate->mFrameBuffer = std::make_shared<FrameBuffer>(canvasSize.x, canvasSize.y);
      }

      const unsigned int texture = mPrivate->mFrameBuffer->texture();

      const ImTextureID textureId(texture);
      ImGui::Image(textureId, canvasSize, ImVec2(0, 1), ImVec2(1, 0));

      if (ImGui::IsWindowHovered())
      {
        handleInput();
      }
    }
  }
  ImGui::End();
  ImGui::PopStyleVar(3); // 弹出样式设置
  return true;
}

void OpenGLViewerWidget::resize(int width, int height) {}

std::shared_ptr<FrameBuffer> OpenGLViewerWidget::renderBuffer()
{
  return mPrivate->mFrameBuffer;
}
bool OpenGLViewerWidget::handle(const EventObject& event)
{
  if (event.eventId() != EventId::SeriesSelected)
  {
    return false;
  }

  const auto eventData = dynamic_cast<const SeriesSelectedData*>(event.eventData());
  if (!eventData)
  {
    return false;
  }
  mPrivate->mDicomSeries = eventData->dicomSeries();
  return false;
}

void OpenGLViewerWidget::mousePressCheck()
{
  if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
  {
    const auto& io = ImGui::GetIO();

    auto buttonPos = io.MouseClickedPos[ImGuiMouseButton_Left];
    auto eventData = std::make_unique<MousePressedData>(buttonPos, ImGuiMouseButton_Left);
    this->invokeEvent(MouseEvent(EventId::MousePressed, std::move(eventData)));

    SPDLOG_DEBUG("mouse left button clicked");
  }
}

void OpenGLViewerWidget::mouseMoveCheck()
{
  const auto& io = ImGui::GetIO();
  if (!ImGui::IsAnyMouseDown() && (fabsf(io.MouseDelta.x) > 1e-6 || fabsf(io.MouseDelta.y) > 1e-6))
  {
    ImVec2 mousePos = io.MousePos;
    ImVec2 preMousePos = io.MousePosPrev;

    auto eventData = std::make_unique<MouseMoveData>(mousePos, preMousePos);
    this->invokeEvent(MouseEvent(EventId::MouseMove, std::move(eventData)));
    SPDLOG_DEBUG("mouse move");
  }
}

void OpenGLViewerWidget::mouseDragCheck()
{
  if (ImGui::IsMouseDragging(ImGuiMouseButton_Left))
  {
    ImVec2 accumulateDelta = ImGui::GetMouseDragDelta(ImGuiMouseButton_Left);
    auto eventData = std::make_unique<MouseDragData>(accumulateDelta, ImGuiMouseButton_Left);
    this->invokeEvent(MouseEvent(EventId::MouseDrag, std::move(eventData)));

    SPDLOG_DEBUG("mouse dragging");
  }
}

void OpenGLViewerWidget::mouseReleaseCheck()
{
  if (ImGui::IsMouseReleased(ImGuiMouseButton_Left))
  {
    this->invokeEvent(MouseEvent(EventId::MouseRelease, std::make_unique<MouseReleaseData>()));
    SPDLOG_DEBUG("mouse left button released");
  }
}

void OpenGLViewerWidget::handleInput()
{
  mousePressCheck();
  mouseDragCheck();
  mouseMoveCheck();
  mouseReleaseCheck();
}
