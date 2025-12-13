#ifndef DCMEVENT_H
#define DCMEVENT_H
#include "EventObject.h"
#include "WidgetsExport.h"

class WIDGETS_API DcmEvent final : public EventObject
{
public:
  DcmEvent(const EventId& eventId, std::unique_ptr<EventData> eventData)
    : EventObject(eventId, std::move(eventData))
  {
  }
};

#endif // DCMEVENT_H
