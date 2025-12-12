#ifndef WIDGET_EVENT_DATA_H
#define WIDGET_EVENT_DATA_H
#include "EventData.h"
#include <imgui.h>
#include "EventMacro.h"
class WidgetResizeData : public EventData
{
public:
  WidgetResizeData(const ImVec2& widgetSize)
    : mWidgetSize(widgetSize)
  {
  }

  ImVec2 widgetSize() const { return mWidgetSize; }

private:
  ImVec2 mWidgetSize;
};

NoParamEventDataDeclaration(DisplayDataNodeTreeData, EventData);
NoParamEventDataDeclaration(DisplayPixelValueData, EventData);
NoParamEventDataDeclaration(DisplayImageNavigator, EventData);
NoParamEventDataDeclaration(DisplayImageProperties, EventData);
NoParamEventDataDeclaration(DisplayLog, EventData);

#endif // WIDGET_EVENT_DATA_H