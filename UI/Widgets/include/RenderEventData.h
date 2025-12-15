#ifndef RENDER_EVENT_DATA_H
#define RENDER_EVENT_DATA_H
#include "EventData.h"
class RenderUpdateData : public EventData
{
public:
  RenderUpdateData() = default;
  ~RenderUpdateData() = default;
};
#endif // RENDER_EVENT_DATA_H