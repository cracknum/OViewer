#ifndef MOUSE_EVENT
#include "EventId.h"
#include "EventObject.h"
#include "MouseEventData.h"
#include <memory>

class MouseEvent final : public EventObject
{
public:
  MouseEvent(const EventId eventId, std::unique_ptr<EventData> eventData)
    : EventObject(eventId, std::move(eventData))
  {
  }
  ~MouseEvent() = default;
};
#endif
