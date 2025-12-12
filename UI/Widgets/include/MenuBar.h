#ifndef MENUBAR_H
#define MENUBAR_H
#include "Widget.h"
#include <memory>
#include <string>
#include <vector>
#include "WidgetsExport.h"
#include "IEventObserver.h"
#include "EventObject.h"
class MenuItemClicked final: public EventObject
{
public:
  MenuItemClicked(const EventId& eventId, std::unique_ptr<EventData> eventData);
  ~MenuItemClicked() override = default;
};

class MenuItemData final: public EventData
{
public:
  explicit MenuItemData(std::string menuItemName);
  ~MenuItemData() override = default;

  [[nodiscard]] std::string menuItemName() const;
private:
  std::string mMenuItemName;
};
class WIDGETS_API MenuBar final : public Widget
{
public:
  class Menu
  {
  public:
    class Item final: public UIObject
    {
    public:
      Item(std::string  name, const std::shared_ptr<IEventObserver>& observer);

    private:
      friend class MenuBar;
      std::string mName;
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
