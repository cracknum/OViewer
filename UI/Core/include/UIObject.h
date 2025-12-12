#ifndef UI_OBJECT_H
#define UI_OBJECT_H
#include "IEventObserver.h"

#include <memory>
#include <vector>
#include "CoreExport.h"
class EventObject;
class CORE_API UIObject
{
public:
  virtual ~UIObject() = default;

  void addObserver(const std::shared_ptr<IEventObserver>& observer);
  void invokeEvent(const EventObject& eventObject);

protected:
  std::vector<std::shared_ptr<IEventObserver>> mObservers;
};

#endif // UI_OBJECT_H
