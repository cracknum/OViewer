#include "Application.hpp"
#include "FrameBuffer.h"
#include "IEventObserver.h"
#include "Log.h"
#include "OpenGLWindow.hpp"
#include "SceneView.hpp"
#include "ShaderProgram.h"
#include "ShaderProgramManager.h"
#include "Vertices.h"
#include <VertexIndexBuffer.h>
#include <spdlog/spdlog.h>
#include <unordered_map>
#include "OpenGLViewerWidget.h"
#include "RenderEvent.h"
#include "ShaderUtils.h"

class RenderObserver : public IEventObserver
{
public:
  RenderObserver(std::shared_ptr<OpenGLViewerWidget> viewerWidget)
    : mViewerWidget(viewerWidget)
  {
    mShaderProgramManager = std::make_shared<ShaderProgramManager>();
    mVertexShaderSource = ShaderUtils::loadShaderSource(R"(D:\Workspace\github\OViewerUI\Test\test.vert)");
    mFragmentShaderSource = ShaderUtils::loadShaderSource( R"(D:\Workspace\github\OViewerUI\Test\test.frag)");
  }
  bool handle(const EventObject& event) override
  {
    auto renderEvent = dynamic_cast<const RenderEvent*>(&event);
    if (!renderEvent || renderEvent->eventId() != EventId::RenderUpdateStart)
    {
      return false;
    }
    std::unordered_map<GLenum, std::string> shaderSources;
    shaderSources[GL_VERTEX_SHADER] = mVertexShaderSource;
    shaderSources[GL_FRAGMENT_SHADER] = mFragmentShaderSource;
    auto shaderProgram = mShaderProgramManager->getShaderProgram(shaderSources);
    auto frameBuffer = mViewerWidget->renderBuffer();

    if (!mVetexIndexBuffer)
    {
      mVetexIndexBuffer = std::make_shared<VertexIndexBuffer>();
      Vertices vertices;
      // clang-format off
    vertices.m_Data = new GLfloat[8]{
      -1.0f, -1.0f,
       1.0f, -1.0f,
       1.0f,  1.0f,
      -1.0f,  1.0f
    };
    vertices.m_Indices = new GLuint[6]{
      0, 1, 3,
      1, 2, 3
    };
      // clang-format on
      vertices.m_DataSize = 8;
      vertices.m_IndicesSize = 6;
      vertices.m_PointAttribute.first = true;
      vertices.m_PointAttribute.second = 2;
      mVetexIndexBuffer->createBuffer(vertices);
    }
    shaderProgram->use();
    frameBuffer->bind();
    mVetexIndexBuffer->draw(GL_TRIANGLES);
    frameBuffer->unbind();
    shaderProgram->unuse();

    return true;
  }

private:
  std::shared_ptr<OpenGLViewerWidget> mViewerWidget;
  std::shared_ptr<ShaderProgramManager> mShaderProgramManager;
  std::shared_ptr<VertexIndexBuffer> mVetexIndexBuffer;
  std::string mVertexShaderSource;
  std::string mFragmentShaderSource;
};

int main(int argc, char** argv)
{
  Application app(argc, argv);
  Log::initLog(spdlog::level::debug);
  auto renderWindow = app.renderWindow();
  // renderWindow->maximum();
  auto glRenderWindow = std::static_pointer_cast<OpenGLWindow>(renderWindow);
  if (!glRenderWindow)
  {
    SPDLOG_DEBUG("opengl window is nullptr");
    return EXIT_FAILURE;
  }

  auto sceneView = glRenderWindow->sceneView();
  if (!sceneView)
  {
    SPDLOG_DEBUG("scene view is nullptr");
    return EXIT_FAILURE;
  }

  auto viewWidget = sceneView->viewerWidget();
  if (!viewWidget)
  {
    SPDLOG_DEBUG("view widget is nullptr");
    return EXIT_FAILURE;
  }
  auto renderObserver = std::make_shared<RenderObserver>(viewWidget);
  glRenderWindow->addObserver(renderObserver);

  return app.exec();
}