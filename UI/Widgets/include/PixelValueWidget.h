#ifndef PIXEL_VALUE_WIDGET
#define PIXEL_VALUE_WIDGET
#include "Widget.h"
#include <memory>
class PixelValueWidget final : public Widget
{
public:
  PixelValueWidget(const char* widgetName, int widgetFlags = 0);
  ~PixelValueWidget();
  bool render() override;
  void resize(int width, int height) override;

private:
  struct Private;
  std::unique_ptr<Private> mPrivate;
};
#endif // PIXEL_VALUE_WIDGET