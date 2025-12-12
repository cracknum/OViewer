#ifndef INTERFACE_EVENT_OBJECT_H
#define INTERFACE_EVENT_OBJECT_H
#include "EventData.h"
#include "EventId.h"
class UIObject;
class EventObject
{
public:
  explicit EventObject(const EventId& eventId, std::unique_ptr<EventData> eventData)
    : mEventId(eventId)
    , mEventData(std::move(eventData))
  {

  }

  virtual ~EventObject() = default;
  [[nodiscard]] EventId eventId() const
  {
    return mEventId;
  }
  [[nodiscard]] const EventData* eventData() const
  {
    return mEventData.get();
  }

protected:
  EventId mEventId;
  std::unique_ptr<EventData> mEventData;
};
#endif // INTERFACE_EVENT_OBJECT_H
