#include <QApplication>
#include "Window.h"
int main(int argc, char** argv)
{
  QApplication app(argc, argv);
  Window window;
  window.resize(1920, 1080);
  window.show();
  return app.exec();
}