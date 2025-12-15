#ifndef INTERFACE_EVENT_LISTENER_H
#define INTERFACE_EVENT_LISTENER_H

class EventObject;
class IEventObserver
{
public:
  virtual bool handle(const EventObject& event) = 0;
  virtual ~IEventObserver() = default;
};

#endif // INTERFACE_EVENT_LISTENER_H
