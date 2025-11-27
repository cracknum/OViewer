#ifndef VIEW_WINDOW_BASE_H
#define VIEW_WINDOW_BASE_H
#include "ViewWindowConfig.h"
#include <QObject>
#include "InteractiveObject.h"
#include <vector>
#include <memory>

class QOpenGLFunctions_4_4_Core;
class ViewWindowBase : public QObject
{
  Q_OBJECT

public:
  explicit ViewWindowBase(
    QOpenGLFunctions_4_4_Core* function, const ViewWindowConfig& config, QObject* parent = nullptr);
  /**
   * @brief 设置view的上下文
   */
  void setFunction(QOpenGLFunctions_4_4_Core* function) { m_Function = function; }
  /**
   * @brief 判断上下文是否有效
   */
  bool isContextVaild() { return m_Function != nullptr; }
  void update();

  /**
   * @brief 绘制窗口, 每次更新都会调用
   */
  virtual void drawWindow() = 0;
  [[nodiscard]] ViewWindowConfig::IdType getWindowId() const;

  /**
   * @brief 根据父窗口的尺寸更新view视图的尺寸
   */
  void updateViewSize(const ViewWindowConfig::WindowSize& parentWindowSize);

  void addPrimitive(std::shared_ptr<InteractiveObject> primitive);

protected:
  QOpenGLFunctions_4_4_Core* m_Function;
  ViewWindowConfig m_Config;
  std::vector<std::shared_ptr<InteractiveObject>> m_Primitives;
};

#endif