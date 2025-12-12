#ifndef IMAGE_PROPERTIES_H
#define IMAGE_PROPERTIES_H
#include "AbstractPropertiesWidget.h"
#include <memory>
#include "WidgetsExport.h"
class WIDGETS_API ImagePropertiesWidget final : public AbstractPropertiesWidget
{
public:
  ImagePropertiesWidget(const char* widgetName, int widgetFlags = 0);
  ~ImagePropertiesWidget() override;

  bool render() override;
  void resize(int width, int height) override;
private:
  struct Private;
  std::unique_ptr<Private> mPrivate;
};

#endif // IMAGE_PROPERTIES_H