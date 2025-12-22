#include "MPRSliceRepresentation.h"
#include <vtkActor.h>
#include <vtkObjectFactory.h>

vtkStandardNewMacro(MPRSliceRepresentation);

struct MPRSliceRepresentation::Private
{
  int mWindow;
  int mLevel;
  Private()
    : mWindow(0)
    , mLevel(0)
  {
  }
};

vtkProp3D* MPRSliceRepresentation::getProp() const {}
void MPRSliceRepresentation::setLevelWindow(int level, int window) {}
void MPRSliceRepresentation::setColorMap(vtkImageMapToColors* colorMap) {}
void MPRSliceRepresentation::BuildRepresentation() {}
MPRSliceRepresentation::MPRSliceRepresentation() {}
MPRSliceRepresentation::~MPRSliceRepresentation() {};