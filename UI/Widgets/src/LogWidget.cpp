#include "LogWidget.h"
#include <imgui.h>
#include "WidgetEvent.h"
#include "WidgetEventData.h"

struct LogWidget::Private
{

};

LogWidget::LogWidget(const char* widgetName, int widgetFlag)
  : Widget(widgetName, widgetFlag)
{
}

LogWidget::~LogWidget() {}

bool LogWidget::render()
{
    if (ImGui::Begin(mWidgetName.c_str(), false, mWidgetFlags))
    {
        auto eventData = std::make_unique<DisplayLog>();
        this->invokeEvent(WidgetEvent(EventId::DisplayLog, std::move(eventData)));
    }
    ImGui::End();    
  return true;
}

void LogWidget::resize(int width, int height) {}
