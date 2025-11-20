#ifndef VIEW_WINDOW_BASE_H
#define VIEW_WINDOW_BASE_H
#include "ViewWindowConfig.h"
#include <QObject>

class QOpenGLContext;
class ViewWindowBase : public QObject
{
  Q_OBJECT

public:
  explicit ViewWindowBase(
    QOpenGLContext* context, const ViewWindowConfig& config, QObject* parent = nullptr);
  /**
   * @brief 设置view的上下文
   */
  void setContext(QOpenGLContext* context) { m_Context = context; }
  /**
   * @brief 判断上下文是否有效
   */
  bool isContextVaild() { return m_Context != nullptr; }
  void update();

  /**
   * @brief 绘制窗口, 每次更新都会调用
   */
  virtual void drawWindow() = 0;

protected:
  QOpenGLContext* m_Context;
  ViewWindowConfig m_Config;
};

#endif