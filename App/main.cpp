#include "Application.hpp"
#include "Log.h"

int main(int argc, char** argv)
{
  Log::initLog(spdlog::level::debug);
  Application app(argc, argv);

  auto executableDir = app.getExecutableDir();
  app.setWindowIcon(executableDir + "/resources/icon/icon.png");
  return app.exec();
}
