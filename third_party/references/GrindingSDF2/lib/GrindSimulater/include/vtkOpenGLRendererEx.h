#ifndef vtkOpenGLRendererEx_h
#define vtkOpenGLRendererEx_h

#ifdef GrindSimulater_EXPORTS
#define GrindSimulater_EXPORT __declspec(dllexport)
#else
#define GrindSimulater_EXPORT __declspec(dllimport)
#endif

#include <vtkOpenGLRenderer.h>

class vtkGrindInteractor;
class GrindSimulater_EXPORT vtkOpenGLRendererEx : public vtkOpenGLRenderer
{
public:
  static vtkOpenGLRendererEx *New();
  vtkTypeMacro(vtkOpenGLRendererEx, vtkOpenGLRenderer);

  vtkGrindInteractor* GetGrindInteractor();
  void DeviceRenderOpaqueGeometry() override;
  void ReleaseGraphicsResources(vtkWindow* w) override;
protected:
  vtkOpenGLRendererEx();
  ~vtkOpenGLRendererEx() override;

  int UpdateOpaquePolygonalGeometry() override;

  int GrindCount;
private:
    class Impl;
    std::unique_ptr<Impl> pImpl;

private:
  vtkOpenGLRendererEx(const vtkOpenGLRendererEx&) = delete;
  void operator=(const vtkOpenGLRendererEx&) = delete;
};

#endif
