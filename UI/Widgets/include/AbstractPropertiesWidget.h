#ifndef ABSTRACT_PROPERTIES_WIDGET
#define ABSTRACT_PROPERTIES_WIDGET
#include "Widget.h"
#include "WidgetsExport.h"
class WIDGETS_API AbstractPropertiesWidget : public Widget
{
public:
  AbstractPropertiesWidget(const char* widgetName, int widgetFlags = 0)
    : Widget(widgetName, widgetFlags)
  {
  }
  virtual bool render() = 0;
  virtual void resize(int width, int height) = 0;
};
#endif // ABSTRACT_PROPERTIES_WIDGET