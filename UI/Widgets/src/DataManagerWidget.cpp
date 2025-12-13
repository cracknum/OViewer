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
};

DataManagerWidget::DataManagerWidget(const char* widgetName, int widgetFlags /*= 0*/)
  : Widget(widgetName, widgetFlags)
{
}

DataManagerWidget::~DataManagerWidget() {}

bool DataManagerWidget::render()
{
  if (ImGui::Begin(mWidgetName.c_str(), false, mWidgetFlags))
  {
    auto eventData = std::make_unique<DisplayDataNodeTreeData>();
    this->invokeEvent(WidgetEvent(EventId::DisplayDataNodeTree, std::move(eventData)));
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
  itk::SmartPointer<DicomSeries> series = seriesSelectData->dicomSeries();
  SPDLOG_DEBUG("select series");
  return false;
}
