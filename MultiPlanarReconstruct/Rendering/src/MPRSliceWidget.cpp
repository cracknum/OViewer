#include "MPRSliceRepresentation.h"
#include "MPRSliceWidget.h"
#include "SliceNavigator.h"
#include <spdlog/spdlog.h>
#include <vtkCommand.h>
#include <vtkImageData.h>
#include <vtkObjectFactory.h>
#include <vtkWidgetCallbackMapper.h>
#include <vtkWidgetEvent.h>


vtkStandardNewMacro(MPRSliceWidget);

void MPRSliceWidget::CreateDefaultRepresentation()
{
  if (!mImageData || !mLevel || !mWindow || mPlaneType == StandardPlane::None)
  {
    SPDLOG_ERROR("imageData, level or window is not setted, initialized falied");
    return;
  }

  if (!this->WidgetRep)
  {
    auto widgetRep = vtkSmartPointer<MPRSliceRepresentation>::New();
    widgetRep->setImageData(mImageData);
    widgetRep->setLevelWindow(mLevel, mWindow);
    widgetRep->setSliceType(mPlaneType);
    this->WidgetRep = widgetRep;
    this->WidgetRep->Register(this);
    this->Modified();
  }
}
void MPRSliceWidget::setLevel(int level)
{
  this->mLevel = level;
  this->Modified();
}
void MPRSliceWidget::setWindow(int window)
{
  this->mWindow = window;
  this->Modified();
}
int MPRSliceWidget::getLevel()
{
  return mLevel;
}
int MPRSliceWidget::getWindow()
{
  return mWindow;
}
void MPRSliceWidget::setImageData(vtkImageData* imageData)
{
  this->mImageData = imageData;
}
MPRSliceWidget::MPRSliceWidget()
  : mLevel(0)
  , mWindow(0)
  , mImageData(nullptr)
  , mPlaneType(StandardPlane::None)
{
  this->CallbackMapper->SetCallbackMethod(vtkCommand::MouseWheelForwardEvent,
    vtkWidgetEvent::Translate, this, MPRSliceWidget::sliceForward);
  /*this->CallbackMapper->SetCallbackMethod(vtkCommand::MouseWheelBackwardEvent,
    vtkWidgetEvent::Translate, this, MPRSliceWidget::sliceBackward);*/
}

void MPRSliceWidget::sliceForward(vtkAbstractWidget* widget)
{
  auto sliceWidget = MPRSliceWidget::SafeDownCast(widget);
  auto widgetRep = MPRSliceRepresentation::SafeDownCast(sliceWidget->WidgetRep);
  auto sliceNavigator = widgetRep->getSliceNavigator();
  sliceNavigator->nextSlice();
  widgetRep->Modified();
}

void MPRSliceWidget::sliceBackward(vtkAbstractWidget* widget)
{
  auto sliceWidget = MPRSliceWidget::SafeDownCast(widget);
  auto widgetRep = MPRSliceRepresentation::SafeDownCast(sliceWidget->WidgetRep);
  auto sliceNavigator = widgetRep->getSliceNavigator();
  //sliceNavigator->previousSlice();
}
MPRSliceWidget::~MPRSliceWidget() {}
void MPRSliceWidget::setSliceType(StandardPlane sliceType)
{
  this->mPlaneType = sliceType;
}
