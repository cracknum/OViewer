#include "Application.hpp"
#include "Log.h"
#include <OpenGLWindow.hpp>

int main(int argc, char** argv)
{
  Log::initLog(spdlog::level::debug);
  Application app(argc, argv);
  auto openglWindow = std::static_pointer_cast<OpenGLWindow>(app.renderWindow());
  openglWindow->sceneView();
  auto executableDir = app.getExecutableDir();
  app.setWindowIcon(executableDir + "/resources/icon/icon.png");
  return app.exec();
}
