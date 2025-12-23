#ifndef SLICE_NAVIGATOR_H
#define SLICE_NAVIGATOR_H
#include "BaseExport.h"
#include "PlaneType.h"
#include <vtkObject.h>
#include <vtkSmartPointer.h>

class SlicedGeometry;
class PlaneGeometry;
class vtkImageData;
/**
 * @brief
 * 管理对应的当前平面对应的SliceGeometry的slice位置、SliceGeometry的初始化、SliceGeometry的重新初始化信息
 * 能够为外部调用者提供当前slice位置的PlaneGeometry
 */
class BASE_API SliceNavigator final : public vtkObject
{
public:
  static SliceNavigator* New();
  vtkTypeMacro(SliceNavigator, vtkObject);

  vtkSetMacro(PlaneType, StandardPlane);
  vtkGetMacro(PlaneType, StandardPlane);
  /**
   * 设置参照数据以及平面类型
   * @param imageData 需要参照的数据
   * @param planeType slice对应的平面类型
   */
  void setReferenceImage(vtkImageData* imageData, StandardPlane planeType);
  PlaneGeometry* getCurrentPlaneGeometry() const;
  /**
   * @brief 设置当前SlicedGeometry对应的位置
   */
  void setCurrentSlicePosition(int sliceNumber);
  void sliceToCenter();

  void nextSlice();
  void previousSlice();

protected:
  SliceNavigator();
  ~SliceNavigator() override;

private:
  StandardPlane PlaneType;
  int CurrentSliceNumber;
  vtkSmartPointer<SlicedGeometry> mSlicedGeometry;
};

#endif // SLICE_NAVIGATOR_H