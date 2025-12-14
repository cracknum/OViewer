#include "ImagePropertiesWidget.h"

#include "DcmEventData.h"
#include "DicomSeries.h"
#include "EventObject.h"

#include <imgui.h>

struct ImagePropertiesWidget::Private
{
  itk::SmartPointer<DicomSeries> mDicomSeries;
  float mWindowWidth;
  float mWindowLevel;
  Private()
    : mWindowWidth(0.0f)
    , mWindowLevel(0.0f)
  {
  }
};

ImagePropertiesWidget::ImagePropertiesWidget(const char* widgetName, int widgetFlags)
  : AbstractPropertiesWidget(widgetName, widgetFlags)
{
  mPrivate = std::make_unique<Private>();
}

ImagePropertiesWidget::~ImagePropertiesWidget() {}

bool ImagePropertiesWidget::render()
{
  if (ImGui::Begin(mWidgetName.c_str(), false, mWidgetFlags))
  {
    if (mPrivate->mDicomSeries)
    {
      if (ImGui::TreeNode("WindowLevel##ImageProperties.WindowLevel"))
      {
        if (ImGui::SliderFloat(
              "window##ImageProperties.WindowLevel", &mPrivate->mWindowWidth, 0, 10000))
        {
        }
        if (ImGui::SliderFloat("level##ImageProperties.WindowLevel", &mPrivate->mWindowLevel,
              mPrivate->mWindowLevel - mPrivate->mWindowWidth * 0.5f,
              mPrivate->mWindowLevel + mPrivate->mWindowWidth * 0.5f))
        {
        }
        ImGui::TreePop();
      }
    }
  }
  ImGui::End();
  return true;
}

void ImagePropertiesWidget::resize(int width, int height) {}
bool ImagePropertiesWidget::handle(const EventObject& event)
{
  if (event.eventId() == EventId::SeriesSelected)
  {
    const auto eventData = dynamic_cast<const SeriesSelectedData*>(event.eventData());
    mPrivate->mDicomSeries = eventData->dicomSeries();
    const auto imageInfo = mPrivate->mDicomSeries->GetImageInfo();
    const auto& windowStr = imageInfo->GetWindowWidth();
    const auto& levelStr = imageInfo->GetWindowCenter();
    mPrivate->mWindowWidth = windowStr.empty() ? 4095.0f : std::stof(windowStr);
    mPrivate->mWindowLevel = levelStr.empty() ? 1024.0f : std::stof(levelStr);
  }

  return false;
}
