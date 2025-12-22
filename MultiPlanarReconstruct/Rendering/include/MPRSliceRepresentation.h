#ifndef MPR_SLICE_REPRESENTATION_H
#define MPR_SLICE_REPRESENTATION_H
#include <memory>
#include <vtkSmartPointer.h>
#include <vtkWidgetRepresentation.h>

class vtkProp3D;
class vtkImageMapToColors;
// TODO: 参考类vtkResliceCursorLineRepresentation类族设计
class MPRSliceRepresentation final : public vtkWidgetRepresentation
{
public:
  static MPRSliceRepresentation* New();
  vtkTypeMacro(MPRSliceRepresentation, vtkObject);

  /**
   * @brief 获取需要渲染的数据
   * @return 需要渲染的数据
   */
  vtkProp3D* getProp() const;
  /**
   * @brief 设置窗位和窗宽
   * @param level 窗位
   * @param window 窗宽
   */
  void setLevelWindow(int level, int window);
  /**
   * @brief 设置颜色映射函数
   * @param colorMap 设置颜色映射函数
   */
  void setColorMap(vtkImageMapToColors* colorMap);
  void BuildRepresentation() override;

protected:
  MPRSliceRepresentation();
  ~MPRSliceRepresentation() override;

private:
  struct Private;
  std::unique_ptr<Private> mPrivate;
};
#endif // MPR_SLICE_REPRESENTATION_H
