#ifndef IMAGE_NAVIGATOR_WIDGET
#define IMAGE_NAVIGATOR_WIDGET
#include "Widget.h"
#include "WidgetsExport.h"
#include <memory>


class WIDGETS_API ImageNavigatorWidget : public Widget
{
public:
  ImageNavigatorWidget(const char* widgetName, int widgetFlags = 0);
  ~ImageNavigatorWidget();

  bool render() override;
  void resize(int width, int height) override;

private:
  struct Private;
  std::unique_ptr<Private> mPrivate;
};
#endif // IMAGE_NAVIGATOR_WIDGET
