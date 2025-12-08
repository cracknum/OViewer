#include "Application.hpp"
#include "Log.h"
// #include "OpenGLActor.h"
//  #include "OpenGLRenderer.h"
//  #include "PlaneSource.h"
//  #include "PolyDataMapper.h"
//  #include "ShaderManager.h"
// #include <OpenGLWindow.hpp>

int main(int argc, char** argv)
{
  Log::initLog(spdlog::level::debug);
  Application app(argc, argv);

  auto executableDir = app.getExecutableDir();
  app.setWindowIcon(executableDir + "/resources/icon/icon.png");
  /* auto renderWindow = app.renderWindow();
   auto glRenderWindow = std::static_pointer_cast<OpenGLWindow>(renderWindow);
   auto sceneView = glRenderWindow->sceneView();
   auto viewerWidget = sceneView->viewerWidget();

   auto shaderManager = std::make_shared<ShaderManager>();
   auto renderer = std::make_shared<OpenGLRenderer>();
   auto planeSource = std::make_shared<PlaneSource>();
   auto mapper = std::make_shared<PolyDataMapper>(shaderManager);
   auto actor = std::make_shared<OpenGLActor>();
   mapper->setDataSource(planeSource);
   actor->setMapper(mapper);
   renderer->addActor(actor);

   viewerWidget->addRenderer(renderer);*/
  return app.exec();
}
