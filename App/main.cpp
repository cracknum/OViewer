#include "Log.h"
#include "Application.hpp"

int main(int argc, char** argv)
{
  Log::initLog(spdlog::level::debug);
  Application app(argc, argv);

  app.setWindowIcon("resources/icon/icon.png");

  return app.exec();
}
