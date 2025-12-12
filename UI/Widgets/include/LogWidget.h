#ifndef LOG_WIDGET
#define LOG_WIDGET
#include "Widget.h"
#include "WidgetsExport.h"
#include <memory>

class WIDGETS_API LogWidget final : public Widget
{
public:
  LogWidget(const char* widgetName, int widgetFlag = 0);
  ~LogWidget();

  bool render() override;
  void resize(int width, int height) override;

private:
  struct Private;
  std::unique_ptr<Private> mPrivate;
};
#endif // LOG_WIDGET