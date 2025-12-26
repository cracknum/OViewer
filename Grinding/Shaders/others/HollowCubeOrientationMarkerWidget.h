#ifndef HOLLOW_CUBE_ORIENTATION_MARKER_WIDGET
#define HOLLOW_CUBE_ORIENTATION_MARKER_WIDGET
#include <vtkOrientationMarkerWidget.h>
#include <vtkSmartPointer.h>

class vtkProp;
class vtkOBBTree;
class vtkAssembly;

class HollowCubeOrientationMarkerWidget : public vtkOrientationMarkerWidget
{
  public:
    static HollowCubeOrientationMarkerWidget* New();
    vtkTypeMacro(HollowCubeOrientationMarkerWidget, vtkOrientationMarkerWidget);
    void PrintSelf(ostream& os, vtkIndent indent) override;

    virtual void SetOrientationMarker(vtkProp* prop) override;

    void SetCubeOBBTree(vtkOBBTree* obbtree);
    void SetEnabled(int) override;

    void SetTextActors(vtkAssembly* textActors);

  private:
    HollowCubeOrientationMarkerWidget();
    ~HollowCubeOrientationMarkerWidget() override;
    void operator=(const HollowCubeOrientationMarkerWidget&) = delete;

    // set up the actors and observers created by this widget
    void SetupWindowInteraction();
    // tear down up the actors and observers created by this widget
    void TearDownWindowInteraction();

  private:
    vtkSmartPointer<vtkAssembly> mTextActors;
};

#endif /*HOLLOW_CUBE_ORIENTATION_MARKER_WIDGET*/
