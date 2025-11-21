#include "ViewWindow.h"
#include "Window.h"
#include <QApplication>
int main(int argc, char** argv)
{
  QApplication app(argc, argv);
  Window window;

  int windowWidth = 1920;
  int windowHeight = 1080;
  ViewWindowConfig config1;
  config1.m_ViewPort = ViewWindowConfig::ViewPort(0, 0, 0.5, 0.5);
  config1.m_BackgroundColor = ViewWindowConfig::Color(1.0f, .0f, .0f, 1.0f);

  ViewWindowConfig config2;
  config2.m_ViewPort =
    ViewWindowConfig::ViewPort(0.5, 0, 0.5, 0.5);
  config2.m_BackgroundColor = ViewWindowConfig::Color(0.0f, 1.0f, .0f, 1.0f);

  ViewWindowConfig config3;
  config3.m_ViewPort =
    ViewWindowConfig::ViewPort(0, 0.5, 0.5, 0.5);
  config3.m_BackgroundColor = ViewWindowConfig::Color(0.0f, 1.0f, .0f, 1.0f);

  ViewWindowConfig config4;
  config4.m_ViewPort = ViewWindowConfig::ViewPort(
    0.5, 0.5, 0.5, 0.5);
  config4.m_BackgroundColor = ViewWindowConfig::Color(1.0f, 0.0f, .0f, 1.0f);

  window.addViewWindow(config1);
  window.addViewWindow(config2);
  window.addViewWindow(config3);
  window.addViewWindow(config4);
  window.resize(windowWidth, windowHeight);
  window.show();
  return app.exec();
}