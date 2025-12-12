#ifndef WIDGET_EVENT_H
#define WIDGET_EVENT_H
#include "EventData.h"
#include "EventObject.h"
#include <memory>

class WidgetEvent : public EventObject
{
public:
  WidgetEvent(const EventId& eventId, std::unique_ptr<EventData> eventData)
    : EventObject(eventId, std::move(eventData))
  {
  }

  ~WidgetEvent() = default;
};

#endif // WIDGET_EVENT_H