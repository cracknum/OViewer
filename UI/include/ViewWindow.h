#ifndef VIEW_WINDOW_H
#define VIEW_WINDOW_H
#include <memory>
#include <QObject>

class vtkFloatArray;
class vtkRenderWindow;
class ViewWindow : public QObject
{
  Q_OBJECT;

public:
  explicit ViewWindow(int viewId, QObject* parent = nullptr);
  ~ViewWindow();

  void setViewSize(vtkFloatArray* viewSize);
  void setBackground(vtkFloatArray* background);
  void addToWindow(vtkRenderWindow* window);
  [[nodiscard]] int getViewId() const;

private:
  struct Impl;
  std::unique_ptr<Impl> m_Impl;
};

#endif // VIEW_WINDOW_H
