#ifndef FILEBROWSER_H
#define FILEBROWSER_H
#include "IEventObserver.h"
#include "Widget.h"
#include "WidgetsExport.h"
#include <memory>
#include <string>

class WIDGETS_API FileBrowserDialog final
  : public IEventObserver
  , public Widget
{
public:
  explicit FileBrowserDialog(const char* widgetName, int widgetFlags = 0);
  ~FileBrowserDialog() override;
  bool render() override;
  void resize(int width, int height) override;
  bool handle(const EventObject& event) override;

private:
  struct Private;
  std::unique_ptr<Private> mPrivate;
};
#endif // FILEBROWSER_H
