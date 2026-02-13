#ifndef UI_EVENT_BUS
#define UI_EVENT_BUS
#include <functional>
#include <QHash>
#include <QObject>
#include <qpointer.h>
/**
 * @brief 插件系统事件总线，便于插件间进行事件交流.
 */
class EventBus final : public QObject
{
  Q_OBJECT;

public:
  explicit EventBus(QObject* parent = nullptr)
    : QObject(parent)
  {
  }
  ~EventBus() = default;

  template <typename Event>
  void subscribe(QObject* subscriber, std::function<void(const Event&)> handler)
  {
    auto key = typeid(Event).name();
    m_Subscribers[key].append(
      { subscriber, [handler](const void* e) { handler(*static_cast<const Event*>(e)); } });
  }

  template <typename Event>
  void invokeEvent(const Event& event)
  {
    auto key = typeid(Event).name();
    for (auto it = m_Subscribers[key].begin(); it != m_Subscribers[key].end(); ++it)
    {
      if (it->m_Subscriber.isNull())
      {
        it = m_Subscribers[key].erase(it);
      }
      else
      {
        it->m_Handler(&event);
        ++it;
      }
    }
  }

private:
  struct Subscription
  {
    QPointer<QObject> m_Subscriber;
    std::function<void(const void*)> m_Handler;
  };

  QHash<QString, QList<Subscription>> m_Subscribers;
};
#endif
