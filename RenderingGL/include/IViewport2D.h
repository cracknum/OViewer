//
// Created by 87068 on 25-12-14.
//

#ifndef INTERFACE_VIEWPORT2D_H
#define INTERFACE_VIEWPORT2D_H
#include "IViewport.h"
#include <vtkMatrix4x4.h>
#include <vtkSmartPointer.h>

class IViewport2D : public IViewport
{
public:
  void setPlaneIndexToWorldMatrix(vtkSmartPointer<vtkMatrix4x4> matrix)
  {
    mPlaneMatrix = std::move(matrix);
  }
  [[nodiscard]] vtkMatrix4x4* planeIndexToWorldMatrix() const { return mPlaneMatrix; }

protected:
  /**
   * @brief set the reslicing plane index to world matrix
   */
  vtkSmartPointer<vtkMatrix4x4> mPlaneMatrix;
};
#endif // INTERFACE_VIEWPORT2D_H
