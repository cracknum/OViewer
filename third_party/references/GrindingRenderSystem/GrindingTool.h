#pragma once
#include <vtkMatrix4x4.h>
#include <vector>

struct ToolParams
{
  enum Type { PLANE = 0, CYLINDER = 1, SPHERE = 2 } type;

  vtkMatrix4x4* Transform;

  // PLANE
  double PlaneNormal[3] = {0,0,1};
  double PlaneOffset = 0.0;

  // CYLINDER
  double CylinderRadius = 1.0;
  double CylinderHeight = 2.0;

  // SPHERE
  double SphereRadius = 1.0;

  ToolParams() { Transform = vtkMatrix4x4::New(); }
  ~ToolParams() { Transform->Delete(); }
};