#ifndef OPENGL_IMGUI_APPLICATION
#define OPENGL_IMGUI_APPLICATION
#include "WidgetsExport.h"
#include <memory>
#include <string>
#include "IWindow.hpp"

struct ApplicationPrivate;

class WIDGETS_API Application
{
public:
    Application(int argc, char **argv);
    ~Application();

    void setWindowIcon(const std::string& iconPath);
    bool exec();

    static std::string getExecutableDir();
    std::shared_ptr<IWindow> renderWindow();

private:
    std::unique_ptr<ApplicationPrivate> mPrivate;
};

#endif // OPENGL_IMGUI_APPLICATION