#ifndef WINDOW_H
#define WINDOW_H
#include "SceneView.hpp"
#include "WidgetsExport.h"
#include <IWindow.hpp>
#include <memory>
#include <string>

struct WindowPrivate;

class WIDGETS_API OpenGLWindow final
  : public IWindow
  , public Widget
{
public:
  OpenGLWindow();
  ~OpenGLWindow() override;

  bool init(int width, int height, const std::string& title);
  bool render() override;
  void handleInput();
  [[nodiscard]] bool isRunning() const;
  void* getNativeWindow() override;
  void setNativeWindow(void* window) override;
  void onResize(int width, int height) override;
  void resize(int width, int height) override;
  void fullScreen() override;
  void maximum() override;
  void close() override;
  void setWindowIcon(const std::string& iconPath) override;
  std::shared_ptr<SceneView> sceneView();
  void exit() override;
  void initMenu();

private:
  std::unique_ptr<WindowPrivate> mPrivate;
};
#endif // WINDOW_H