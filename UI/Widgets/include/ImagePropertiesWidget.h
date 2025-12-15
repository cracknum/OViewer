#ifndef IMAGE_PROPERTIES_H
#define IMAGE_PROPERTIES_H
#include "AbstractPropertiesWidget.h"
#include "IEventObserver.h"
#include "WidgetsExport.h"
#include <memory>
class WIDGETS_API ImagePropertiesWidget final
  : public AbstractPropertiesWidget
  , public IEventObserver
{
public:
  ImagePropertiesWidget(const char* widgetName, int widgetFlags = 0);
  ~ImagePropertiesWidget() override;

  bool render() override;
  void resize(int width, int height) override;
  bool handle(const EventObject& event) override;

private:
  struct Private;
  std::unique_ptr<Private> mPrivate;
};

#endif // IMAGE_PROPERTIES_H