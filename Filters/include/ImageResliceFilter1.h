#ifndef IMAGERESLICEFILTER1_H
#define IMAGERESLICEFILTER1_H
#include <array>
#include <memory>
#include <vector>
#include <vtkImageData.h>
#include <vtkSmartPointer.h>
#include "FiltersExport.h"

struct Volume;
class Texture2DObject;
class vtkTransform;
class vtkPoints;
class vtkMatrix4x4;
class FILTERS_API ImageResliceFilter1
{
public:
  ImageResliceFilter1();
  ~ImageResliceFilter1();
  void setTexture(std::shared_ptr<Texture2DObject> textureObject);
  bool doFilter(vtkSmartPointer<vtkImageData> volume, vtkSmartPointer<vtkTransform> planeLocalToWorldTransform);
protected:
  vtkSmartPointer<vtkPoints> calculatePlaneLocalBounds(vtkImageData* volume, vtkTransform* planeLocalToWorldTransform);
  int calculateMaxTextureSize(vtkImageData* volume);
  static bool LineIntersectZero(vtkPoints* points, int p1, int p2, double* bounds);
private:
  struct Private;
  std::unique_ptr<Private> mPrivate;
};

#endif // IMAGERESLICEFILTER1_H
