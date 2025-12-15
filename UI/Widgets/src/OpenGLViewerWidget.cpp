#include "OpenGLViewerWidget.h"
#include "DcmEvent.h"
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

  Private() { mFrameBuffer = std::make_shared<FrameBuffer>(1, 1); }
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
    unsigned int texture = mPrivate->mFrameBuffer->texture();
    int width = mPrivate->mFrameBuffer->textureWidth();
    int height = mPrivate->mFrameBuffer->textureHeight();

    if (texture != 0 & width > 0 & height > 0)
    {
      ImVec2 canvasSize = ImGui::GetContentRegionAvail();
      if (canvasSize.x <= 0 || canvasSize.y <= 0)
        return false;

      ImTextureID textureId(texture);
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

void OpenGLViewerWidget::resize(int width, int height)
{
  if (mPrivate->mFrameBuffer->textureWidth() - width != 0 ||
    mPrivate->mFrameBuffer->textureHeight() - height != 0)
  {
    mPrivate->mFrameBuffer->updateBufferSize(width, height);
    SPDLOG_DEBUG("resize framebuffer");
  }
}

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
    auto& io = ImGui::GetIO();

    auto buttonPos = io.MouseClickedPos[ImGuiMouseButton_Left];
    auto eventData = std::make_unique<MousePressedData>(buttonPos, ImGuiMouseButton_Left);
    this->invokeEvent(MouseEvent(EventId::MousePressed, std::move(eventData)));

    SPDLOG_DEBUG("mouse left button clicked");
  }
}

void OpenGLViewerWidget::mouseMoveCheck()
{
  auto& io = ImGui::GetIO();
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
    auto& io = ImGui::GetIO();
    ImVec2 accumulateDelta = ImGui::GetMouseDragDelta(ImGuiMouseButton_Left);
    auto eventData = std::make_unique<MouseDragData>(accumulateDelta, ImGuiMouseButton_Left);
    this->invokeEvent(MouseEvent(EventId::MouseDrag, std::move(eventData)));

    SPDLOG_DEBUG("mouse draging");
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
