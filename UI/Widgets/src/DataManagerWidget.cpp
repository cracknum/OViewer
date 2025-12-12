#include "DataManagerWidget.h"
#include "WidgetEvent.h"
#include "WidgetEventData.h"
#include <imgui.h>
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
