#pragma once
#include "GrindingTool.h"
#include <vtkTextureObject.h>
#include <vtkOpenGLRenderWindow.h>
#include <vector>

class GrindingComputeUpdater
{
public:
  GrindingComputeUpdater(vtkOpenGLRenderWindow* renWin);
  ~GrindingComputeUpdater();

  void UpdateRemovalVolume(
    vtkTextureObject* volume,
    const std::vector<ToolParams>& tools,
    const double volumeMin[3],
    const double volumeMax[3]);

private:
  void CreateShader();
  void UploadToolData(const std::vector<ToolParams>& tools, const double volumeMin[3], const double volumeMax[3]);

  vtkOpenGLRenderWindow* RenderWindow;
  GLuint ComputeProgram = 0;
  GLuint ToolSSBO = 0;

  static constexpr int MAX_TOOLS = 16;
};