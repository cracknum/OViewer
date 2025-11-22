#ifndef INTERACTIVE_OBJECT_H
#define INTERACTIVE_OBJECT_H
#include "GLObjectExport.h"
class QMouseEvent;
class QWheelEvent;
class QKeyEvent;

class GLOBJECT_API InteractiveObject
{
public:
  virtual ~InteractiveObject() = default;
  virtual void mousePressEvent(QMouseEvent* event) = 0;
  virtual void mouseReleaseEvent(QMouseEvent* event) = 0;
  virtual void mouseDoubleClickEvent(QMouseEvent* event) = 0;
  virtual void mouseMoveEvent(QMouseEvent* event) = 0;
  virtual void wheelEvent(QWheelEvent* event) = 0;
  virtual void keyPressEvent(QKeyEvent* event) = 0;
  virtual void keyReleaseEvent(QKeyEvent* event) = 0;
  virtual void draw() = 0;
};

#endif //INTERACTIVE_OBJECT_H
