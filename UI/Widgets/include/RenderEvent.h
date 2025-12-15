#ifndef RENDER_EVENT_H
#define RENDER_EVENT_H
#include "EventData.h"
#include "EventId.h"
#include "EventObject.h"
#include <memory>

class RenderEvent : public EventObject
{
public:
  RenderEvent(const EventId& eventId, std::unique_ptr<EventData> eventData)
    : EventObject(eventId, std::move(eventData))
  {
  }
  ~RenderEvent() = default;
};
#endif // APP_EVENT_H