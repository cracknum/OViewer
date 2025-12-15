#ifndef WIDGET_H
#define WIDGET_H
#include "UIObject.h"
#include "WidgetsExport.h"
#include <string>
class WIDGETS_API Widget : public UIObject
{
public:
  Widget(const char* widgetName, int widgetFlags = 0)
    : mWidgetName(widgetName)
    , mWidgetFlags(widgetFlags)
  {
  }
  virtual bool render() = 0;
  virtual void resize(int width, int height) = 0;
  ~Widget() override = default;

protected:
  std::string mWidgetName;
  int mWidgetFlags;
};

#endif // WIDGET_H
