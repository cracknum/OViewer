#ifndef WIDGET_EVENT_DATA_H
#define WIDGET_EVENT_DATA_H
#include "EventData.h"
#include "EventMacro.h"
#include <imgui.h>
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

class MenuItemData final : public EventData
{
public:
  explicit MenuItemData(std::string menuItemName)
    : mMenuItemName(std::move(menuItemName))
  {
  }
  ~MenuItemData() override = default;

  [[nodiscard]] std::string menuItemName() const { return mMenuItemName; }

private:
  std::string mMenuItemName;
};

class FileOpenedData final : public EventData
{
public:
  explicit FileOpenedData(std::string filePath)
    : mFilePath(std::move(filePath))
  {
  }
  [[nodiscard]] std::string filePath() const { return mFilePath; }
  ~FileOpenedData() override = default;

private:
  std::string mFilePath;
};
#endif // WIDGET_EVENT_DATA_H