#ifndef MOUSE_EVENT_DATA
#define MOUSE_EVENT_DATA
#include "EventData.h"
#include <imgui.h>

class MousePressedData : public EventData
{
public:
  MousePressedData(const ImVec2& globalPos, const ImGuiMouseButton& mouseButton)
    : mGlobalPos(globalPos)
    , mMouseButton(mouseButton)
  {
  }
  ~MousePressedData() = default;
  ImVec2 globalPos() { return mGlobalPos; }
  ImGuiButtonFlags mouseButton() { return mMouseButton; }

private:
  ImVec2 mGlobalPos;
  ImGuiButtonFlags mMouseButton;
};

class MouseMoveData : public EventData
{
public:
  MouseMoveData(const ImVec2& globalPos, const ImVec2& oldGloablPos)
    : mGlobalPos(globalPos)
    , mOldGlobalPos(oldGloablPos)
  {
  }
  ~MouseMoveData() = default;

  ImVec2 globalPos() { return mGlobalPos; }
  ImVec2 oldGlobalPos() { return mOldGlobalPos; }

private:
  ImVec2 mGlobalPos;
  ImVec2 mOldGlobalPos;
};

class MouseReleaseData : public EventData
{
public:
  MouseReleaseData() = default;
  ~MouseReleaseData() = default;
};

class WheelData : public EventData
{
public:
  WheelData() = default;
  ~WheelData() = default;
};

class MouseDragData : public EventData
{
public:
  /**
   * @param accuDelta 累计的从点击开始到最终取消拖动的总的delta
   */
  MouseDragData(const ImVec2& accuDelta, const ImGuiMouseButton& mouseButton)
    : mAccuDelta(accuDelta)
    , mMouseButton(mouseButton)
  {
  }
  ~MouseDragData() = default;

private:
  ImVec2 mAccuDelta;
  ImGuiMouseButton mMouseButton;
};
#endif // MOUSE_EVENT_DATA