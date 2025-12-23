#ifndef SLICE_NAVIGATOR_H
#define SLICE_NAVIGATOR_H
#include "PlaneType.h"
#include <vtkObject.h>
#include "BaseExport.h"

class PlaneGeometry;
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

  PlaneGeometry* getCurrentPlaneGeometry() const;
  /**
   * @brief 设置当前SlicedGeometry对应的位置
   */
  void setCurrentSlicePosition(int sliceNumber);
protected:
  SliceNavigator();
  ~SliceNavigator() override;

private:
  StandardPlane PlaneType;
  int CurrentSliceNumber;
};

#endif // SLICE_NAVIGATOR_H