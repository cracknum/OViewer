#include "DataManagerWidget.h"
#include "DcmEvent.h"
#include "DcmEventData.h"
#include "DicomSeries.h"
#include "WidgetEvent.h"
#include "WidgetEventData.h"
#include <imgui.h>
#include <spdlog/spdlog.h>
#include <string>

struct DataManagerWidget::Private
{
  itk::SmartPointer<DicomSeries> mSeries;
  int mSelectIndex;

  Private()
    : mSelectIndex(-1)
  {
  }
};

DataManagerWidget::DataManagerWidget(const char* widgetName, int widgetFlags /*= 0*/)
  : Widget(widgetName, widgetFlags)
{
  mPrivate = std::make_unique<Private>();
}

DataManagerWidget::~DataManagerWidget() {}

bool DataManagerWidget::render()
{
  if (ImGui::Begin(mWidgetName.c_str(), false, mWidgetFlags))
  {
    if (ImGui::TreeNode("DataManager##dataManager"))
    {
      if (mPrivate->mSeries)
      {
        const auto studyInfo = mPrivate->mSeries->GetStudyInfo();
        const auto seriesInfo = mPrivate->mSeries->GetSeriesInfo();
        const auto imageInfo = mPrivate->mSeries->GetImageInfo();
        const auto equipInfo = mPrivate->mSeries->GetEquipInfo();
        const auto desc = studyInfo->GetDescription() + " " + seriesInfo->GetNumber() + " " +
          equipInfo->GetSliceThickness() + "##row_0";

        if (ImGui::Selectable(desc.c_str(), mPrivate->mSelectIndex == 0))
        {
          auto eventData = std::make_unique<SeriesSelectedData>(mPrivate->mSeries);
          this->invokeEvent(DcmEvent(EventId::SeriesSelected, std::move(eventData)));
          mPrivate->mSelectIndex = 0;
        }
      }
      ImGui::TreePop();
    }
  }
  ImGui::End();

  return true;
}

void DataManagerWidget::resize(int width, int height) {}
bool DataManagerWidget::handle(const EventObject& event)
{
  if (event.eventId() != EventId::SeriesSelected)
  {
    return false;
  }
  auto seriesSelectEvent = dynamic_cast<const DcmEvent*>(&event);
  auto seriesSelectData = dynamic_cast<const SeriesSelectedData*>(seriesSelectEvent->eventData());
  mPrivate->mSeries = seriesSelectData->dicomSeries();
  mPrivate->mSelectIndex = -1;
  return false;
}
