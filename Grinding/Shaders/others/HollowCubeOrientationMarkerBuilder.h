#ifndef HOLLOW_CUBE_ORIENTATION_MARKER_BUILDER
#define HOLLOW_CUBE_ORIENTATION_MARKER_BUILDER
#include "CubeOrientationMarker.h"
#include <vtkSmartPointer.h>

class vtkRenderWindowInteractor;
class vtkProp;
class vtkAssembly;
class HollowCubeOrientationMarkerWidget;

class HollowCubeOrientationMarker : public CubeOrientationMarkerBuilder
{
  public:
    HollowCubeOrientationMarker(vtkRenderWindowInteractor* interactor);
    ~HollowCubeOrientationMarker() override;

    vtkSmartPointer<HollowCubeOrientationMarkerWidget> Build();

    void AddVectorText(vtkAssembly* assembly, const char* text, double* origin, double scale);
    void AddVectorTexts(vtkAssembly* assembly);
};
#endif
