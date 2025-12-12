#include "UIObject.h"
#include <spdlog/spdlog.h>
void UIObject::addObserver(const std::shared_ptr<IEventObserver>& observer)
{
  mObservers.push_back(observer);
}
void UIObject::invokeEvent(const EventObject& eventObject)
{
  for (const auto& observer: mObservers)
  {
    if (observer->handle(eventObject))
    {
      return;
    }
  }
}