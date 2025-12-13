#ifndef MENUBAR_H
#define MENUBAR_H
#include "EventId.h"
#include "EventObject.h"
#include "IEventObserver.h"
#include "Widget.h"
#include "WidgetsExport.h"
#include <memory>
#include <string>
#include <vector>

class WIDGETS_API MenuBar final : public Widget
{
public:
  class Menu
  {
  public:
    class Item final : public UIObject
    {
    public:
      Item(std::string name, const EventId& bindEvent,
        const std::shared_ptr<IEventObserver>& observer);

    private:
      friend class MenuBar;
      std::string mName;
      EventId mBindEventId;
    };

  public:
    void setName(const std::string& name);
    void addItem(const Item& item);

  private:
    friend class MenuBar;
    std::string mName;
    std::vector<Item> mMenuItems;
  };

public:
  MenuBar(const char* widgetName = "", int widgetFlags = 0);
  ~MenuBar() override;
  bool render() override;
  void resize(int width, int height) override;

  void addMenu(const Menu& item);

private:
  struct Impl;
  std::unique_ptr<Impl> mImpl;
};

#endif // MENUBAR_H
