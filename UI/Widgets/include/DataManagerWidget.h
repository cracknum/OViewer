#ifndef DATA_MANAGER_WIDGET_H
#define DATA_MANAGER_WIDGET_H
#include "IEventObserver.h"
#include "Widget.h"
#include "WidgetsExport.h"
#include <memory>

class WIDGETS_API DataManagerWidget final
  : public Widget
  , public IEventObserver
{
public:
  DataManagerWidget(const char* widgetName, int widgetFlags = 0);
  ~DataManagerWidget() override;
  bool render() override;
  void resize(int width, int height) override;
  bool handle(const EventObject& event) override;

private:
  struct Private;
  std::unique_ptr<Private> mPrivate;
};
#endif // DATA_MANAGER_WIDGET_H