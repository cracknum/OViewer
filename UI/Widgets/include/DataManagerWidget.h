#ifndef DATA_MANAGER_WIDGET_H
#define DATA_MANAGER_WIDGET_H
#include "Widget.h"
#include <memory>
#include "WidgetsExport.h"

class WIDGETS_API DataManagerWidget final : public Widget
{
public:
  DataManagerWidget(const char* widgetName, int widgetFlags = 0);
  ~DataManagerWidget() override;
  bool render() override;
  void resize(int width, int height) override;
private:
  struct Private;
  std::unique_ptr<Private> mPrivate;
};
#endif // DATA_MANAGER_WIDGET_H