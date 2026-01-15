#include "HollowCubeOrientationMarkerWidget.h"
#include <vtkAssembly.h>
#include <vtkCallbackCommand.h>
#include <vtkCamera.h>
#include <vtkCommand.h>
#include <vtkFollower.h>
#include <vtkIdList.h>
#include <vtkLightKit.h>
#include <vtkOBBTree.h>
#include <vtkObjectFactory.h>
#include <vtkPoints.h>
#include <vtkPropCollection.h>
#include <vtkProperty.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkSmartPointer.h>

class vtkOrientationMarkerWidgetObserver : public vtkCommand
{
  public:
    static vtkOrientationMarkerWidgetObserver* New()
    {
        return new vtkOrientationMarkerWidgetObserver;
    }

    vtkOrientationMarkerWidgetObserver() { this->OrientationMarkerWidget = nullptr; }

    void Execute(vtkObject* wdg, unsigned long event, void* calldata) override
    {
        if (this->OrientationMarkerWidget)
        {
            this->OrientationMarkerWidget->ExecuteCameraUpdateEvent(wdg, event, calldata);
        }
    }

    vtkOrientationMarkerWidget* OrientationMarkerWidget;
};

class CheckTextVisibleCommand : public vtkCommand
{
  public:
    vtkTypeMacro(CheckTextVisibleCommand, vtkCommand);
    static CheckTextVisibleCommand* New();

    virtual void Execute(vtkObject* caller, unsigned long eventId, void* callData)
    {
        if (eventId == vtkCommand::LeftButtonPressEvent)
        {
            mLeftButtonPressed = true;
        }
        else if (eventId == vtkCommand::EndInteractionEvent)
        {
            mLeftButtonPressed = false;
        }

        if (eventId == vtkCommand::InteractionEvent)
        {
            if (false == mLeftButtonPressed)
            {
                return;
            }

            setStatus(mRenderer, mCubeObbTree);
        }
        else if (eventId == vtkCommand::ResetCameraEvent)
        {
            setStatus(mRenderer, mCubeObbTree);
        }
    }

    void SetRenderer(vtkRenderer* renderer) { mRenderer = renderer; }

    void SetCubeObb(vtkOBBTree* cubeObbTree) { mCubeObbTree = cubeObbTree; }

    void setStatus(vtkRenderer* renderer, vtkOBBTree* cubeObbTree)
    {
        vtkCamera* camera = renderer->GetActiveCamera();
        double* cameraPos = camera->GetPosition();

        vtkActorCollection* collection = renderer->GetActors();
        collection->InitTraversal();
        vtkIdType numOfActors = collection->GetNumberOfItems();
        double followerOrigin[3], bounds[6];
        double line[6] = { cameraPos[0], cameraPos[1], cameraPos[2] };
        for (vtkIdType i = 0; i < numOfActors; i++)
        {
            vtkFollower* follower = vtkFollower::SafeDownCast(collection->GetNextActor());
            if (nullptr != follower)
            {
                follower->GetBounds(bounds);
                follower->GetOrigin(followerOrigin);
                line[3] = followerOrigin[0];
                line[4] = followerOrigin[1];
                line[5] = followerOrigin[2];

                bool intersected = intersectWithCube(cubeObbTree, line);
                if (true == intersected)
                {
                    follower->GetProperty()->SetOpacity(0);
                }
                else
                {
                    follower->GetProperty()->SetOpacity(1);
                }
            }
        }
        renderer->Render();
    }

    bool intersectWithCube(vtkOBBTree* cubeObb, double* line)
    {
        double linePoint1[3] = { line[0], line[1], line[2] };
        double linePoint2[3] = { line[3], line[4], line[5] };

        vtkNew<vtkPoints> tsPoints;
        vtkNew<vtkIdList> idlist;
        bool intersect = cubeObb->IntersectWithLine(linePoint2, linePoint1, tsPoints, idlist);

        return intersect;
    }

  protected:
    CheckTextVisibleCommand() { mLeftButtonPressed = false; };
    ~CheckTextVisibleCommand() override {};

  private:
    bool mLeftButtonPressed;
    vtkRenderer* mRenderer;
    vtkSmartPointer<vtkOBBTree> mCubeObbTree;
};

vtkStandardNewMacro(CheckTextVisibleCommand) vtkStandardNewMacro(HollowCubeOrientationMarkerWidget);

HollowCubeOrientationMarkerWidget::HollowCubeOrientationMarkerWidget()
{
    mTextActors = nullptr;
    SetViewport(0, 0, 0.3, 0.3);

    vtkNew<vtkLightKit> lightKit;
    lightKit->AddLightsToRenderer(this->Renderer);
    this->Renderer->GetActiveCamera()->SetParallelProjection(false);
}

HollowCubeOrientationMarkerWidget::~HollowCubeOrientationMarkerWidget()
{
    TearDownWindowInteraction();
}

void HollowCubeOrientationMarkerWidget::SetupWindowInteraction()
{
    vtkRenderWindow* renwin = this->CurrentRenderer->GetRenderWindow();
    vtkCamera* cam = this->Renderer->GetActiveCamera();
    renwin->AddRenderer(this->Renderer);
    if (renwin->GetNumberOfLayers() < 2)
    {
        renwin->SetNumberOfLayers(2);
    }

    // this->CurrentRenderer->AddViewProp(this->OutlineActor);

    // setup helper text follower
    if (nullptr != mTextActors)
    {
        vtkNew<vtkPropCollection> propCollection;
        mTextActors->GetActors(propCollection);
        propCollection->InitTraversal();

        for (size_t i = 0; i < propCollection->GetNumberOfItems(); i++)
        {
            auto* follower = vtkFollower::SafeDownCast(propCollection->GetNextProp());
            follower->SetCamera(cam);
            follower->SetUseBounds(false);
            this->Renderer->AddViewProp(follower);
        }
    }

    this->Renderer->AddViewProp(this->OrientationMarker);
    this->OrientationMarker->VisibilityOn();

    if (this->Interactive)
    {
        vtkRenderWindowInteractor* interactor = this->Interactor;
        if (this->EventCallbackCommand)
        {
            interactor->AddObserver(
              vtkCommand::MouseMoveEvent, this->EventCallbackCommand, this->Priority);
            interactor->AddObserver(
              vtkCommand::LeftButtonPressEvent, this->EventCallbackCommand, this->Priority);
            interactor->AddObserver(
              vtkCommand::LeftButtonReleaseEvent, this->EventCallbackCommand, this->Priority);
        }
    }

    // cam->ParallelProjectionOn();
    // We need to copy the camera before the compositing observer is called.
    // Compositing temporarily changes the camera to display an image.
    this->StartEventObserverId =
      this->CurrentRenderer->AddObserver(vtkCommand::StartEvent, this->Observer, 1);
}

void HollowCubeOrientationMarkerWidget::TearDownWindowInteraction()
{
    if (this->StartEventObserverId != 0)
    {
        this->CurrentRenderer->RemoveObserver(this->StartEventObserverId);
    }

    this->Interactor->RemoveObserver(this->EventCallbackCommand);

    this->OrientationMarker->VisibilityOff();
    this->Renderer->RemoveViewProp(this->OrientationMarker);

    if (nullptr != mTextActors)
    {
        vtkNew<vtkPropCollection> propCollection;
        mTextActors->GetActors(propCollection);
        propCollection->InitTraversal();

        for (size_t i = 0; i < propCollection->GetNumberOfItems(); i++)
        {
            auto* follower = vtkFollower::SafeDownCast(propCollection->GetNextProp());
            this->Renderer->RemoveActor(follower);
        }
    }

    // this->CurrentRenderer->RemoveViewProp(this->OutlineActor);

    // if the render window is still around, remove our renderer from it
    vtkRenderWindow* renwin = this->CurrentRenderer->GetRenderWindow();
    if (renwin)
    {
        renwin->RemoveRenderer(this->Renderer);
    }
}

void HollowCubeOrientationMarkerWidget::PrintSelf(ostream& os, vtkIndent indent) {}

void HollowCubeOrientationMarkerWidget::SetOrientationMarker(vtkProp* prop)
{
    vtkAssembly* assembly = vtkAssembly::SafeDownCast(prop);
    if (nullptr != assembly)
    {
        vtkNew<vtkPropCollection> propCollection;
        assembly->GetActors(propCollection);
        propCollection->InitTraversal();
        for (size_t i = 0; i < propCollection->GetNumberOfItems(); i++)
        {
            vtkFollower* follower = vtkFollower::SafeDownCast(propCollection->GetNextProp());
            if (nullptr == follower)
            {
                continue;
            }

            follower->SetCamera(this->Renderer->GetActiveCamera());
        }
    }

    Superclass::SetOrientationMarker(prop);
}

#include <vtkRendererCollection.h>
void HollowCubeOrientationMarkerWidget::SetCubeOBBTree(vtkOBBTree* obbtree)
{
    vtkNew<CheckTextVisibleCommand> checkTextVisibleCommand;

    checkTextVisibleCommand->SetRenderer(this->Renderer);
    checkTextVisibleCommand->SetCubeObb(obbtree);

    Interactor->AddObserver(vtkCommand::LeftButtonPressEvent, checkTextVisibleCommand);
    Interactor->AddObserver(vtkCommand::EndInteractionEvent, checkTextVisibleCommand);
    Interactor->AddObserver(vtkCommand::InteractionEvent, checkTextVisibleCommand);

    /*Interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->AddObserver(
      vtkCommand::ResetCameraEvent, checkTextVisibleCommand);*/
}

void HollowCubeOrientationMarkerWidget::SetEnabled(int value)
{
    if (!this->Interactor)
    {
        vtkErrorMacro("The interactor must be set prior to enabling/disabling widget");
    }

    if (value != this->Enabled)
    {
        if (value)
        {
            if (!this->OrientationMarker)
            {
                vtkErrorMacro(
                  "An orientation marker must be set prior to enabling/disabling widget");
                return;
            }

            if (!this->CurrentRenderer)
            {
                int* pos = this->Interactor->GetLastEventPosition();
                this->SetCurrentRenderer(this->Interactor->FindPokedRenderer(pos[0], pos[1]));

                if (this->CurrentRenderer == nullptr)
                {
                    return;
                }
            }

            this->UpdateInternalViewport();

            this->SetupWindowInteraction();
            this->Enabled = 1;
            this->InvokeEvent(vtkCommand::EnableEvent, nullptr);
        }
        else
        {
            this->InvokeEvent(vtkCommand::DisableEvent, nullptr);
            this->Enabled = 0;
            this->TearDownWindowInteraction();
            this->SetCurrentRenderer(nullptr);
        }
    }
}

void HollowCubeOrientationMarkerWidget::SetTextActors(vtkAssembly* textActors)
{
    mTextActors = textActors;
}
