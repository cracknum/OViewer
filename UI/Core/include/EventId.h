#ifndef EVENT_ID_H
#define EVENT_ID_H
#include <string>
// clang-format off
enum class EventId
{
  //-------------- menu event-------------------
  FileOpened,                                     // this event called when menu file opened button pressed
  FileClosed,                                     // this event called when menu file closed button pressed
  MenuItemClicked,                                // this event called when menu item clicked
  Exit,                                           // this event called when menu app exit button pressed
  //-------------- menu event-------------------
  //-------------- render event-------------------
  RenderUpdateStart,                              // this event called when start render update
  RenderUpdateEnd,                                // this event called when finish render update
  //-------------- mouse event------------------
  MousePressed,                                   // this event called when mouse pressed
  MouseMove,                                      // this event called when mouse move
  MouseRelease,                                   // this event called when mouse release
  MouseDrag,                                      // this event called when mouse draging
  WheelForward,                                   // this event called when wheel forward
  WheelBackward,                                  // this event called when wheel backward
  //-------------- mouse event------------------
  //-------------- widget event-----------------
  WidgetResize,                                   // this event called when widget resize
  //-------------- widget event-----------------
  DisplayDataNodeTree,                            // this event called when display data manager data node tree panel
  DisplayPixelValue,                              // this event called when display pixel value properties panel
  DisplayImageNavigator,                          // this event called when display image navigator panel
  DisplayImageProperties,                         // this event called when display image properties panel
  DisplayLog,                                     // this event called when display log panel
  //-------------- custom event------------------
  CustomEvent = 1000                              // if you need custom event, event id shoudle greater than 1000
};

inline std::vector<std::string> EventIdStr = { 
  "FileOpened", "FileClosed", "MenuItemClicked",
  "RenderUpdateStart", "RenderUpdateEnd",
  "MousePressed", "MouseMove", "MouseRelease", "MouseDrag", "MouseForward", "MouseBackward",
  "WidgetResize",
  "DisplayDataNodeTree", "DisplayPixelValue", "DisplayImageNavigator", "DisplayImageProperties", "DisplayLog",
  "Exit"
};

// clang-format on

#endif // EVENT_ID_H
