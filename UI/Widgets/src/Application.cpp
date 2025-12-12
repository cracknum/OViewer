#include "Application.hpp"
#include "OpenGLWindow.hpp"
#include <iostream>

#include <filesystem>
#include <string>

#ifdef _WIN32
#include <limits.h>
#include <windows.h>

#elif __APPLE__
#include <limits.h>
#include <mach-o/dyld.h>

#elif __linux__
#include <limits.h>
#include <unistd.h>

#else
#error "Unsupported platform"
#endif

namespace fs = std::filesystem;
#define PATH_MAX 1024

namespace
{
std::string getExecutablePath()
{
  char buffer[PATH_MAX] = { 0 };

#ifdef _WIN32
  GetModuleFileNameA(nullptr, buffer, sizeof(buffer));
#elif __APPLE__
  uint32_t size = sizeof(buffer);
  if (_NSGetExecutablePath(buffer, &size) != 0)
  {
    // Buffer too small, but unlikely with PATH_MAX
    return "";
  }
#elif __linux__
  ssize_t len = readlink("/proc/self/exe", buffer, sizeof(buffer) - 1);
  if (len != -1)
  {
    buffer[len] = '\0';
  }
  else
  {
    return "";
  }
#endif
  auto path = std::string(buffer);
  std::replace(path.begin(), path.end(), '\\', '/');

  return path;
}
}

struct ApplicationPrivate
{
  std::shared_ptr<OpenGLWindow> mRenderWindow;

  ApplicationPrivate() {}
};

Application::Application(int argc, char** argv)
{
  mPrivate = std::make_unique<ApplicationPrivate>();
  mPrivate->mRenderWindow = std::make_shared<OpenGLWindow>();
  mPrivate->mRenderWindow->init(1920, 1080, "Window");
}

Application::~Application() {}

void Application::setWindowIcon(const std::string& iconPath)
{
  mPrivate->mRenderWindow->setWindowIcon(iconPath);
}

bool Application::exec()
{
  while (mPrivate->mRenderWindow->isRunning())
  {
    mPrivate->mRenderWindow->render();
  }

  return true;
}

std::string Application::getExecutableDir()
{
  std::string exePath = getExecutablePath();
  if (exePath.empty())
    return "";
  return fs::path(exePath).parent_path().string();
}

std::shared_ptr<IWindow> Application::renderWindow()
{
  return mPrivate->mRenderWindow;
}
