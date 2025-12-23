#ifndef MPR_SLICE_REPRESENTATION_H
#define MPR_SLICE_REPRESENTATION_H
#include <memory>
#include <vtkSmartPointer.h>
#include <vtkWidgetRepresentation.h>
#include "PlaneType.h"

class vtkMatrix4x4;
class vtkImageAlgorithm;
class vtkPlaneSource;
class vtkActor;
class vtkTexture;
class vtkPolyDataMapper;
class vtkImageMapToColors;
class vtkLookupTable;
class vtkImageData;
class SliceNavigator;

// TODO: 参考类vtkResliceCursorLineRepresentation类族设计
class MPRSliceRepresentation : public vtkWidgetRepresentation
{
public:
  static MPRSliceRepresentation* New();
  void PrintSelf(ostream& os, vtkIndent indent) override;

  vtkTypeMacro(MPRSliceRepresentation, vtkObject);
  void setImageData(vtkImageData* imageData);
  void setSliceType(StandardPlane planeType);

  /**
   * @brief 设置窗位和窗宽
   * @param level 窗位
   * @param window 窗宽
   */
  void setLevelWindow(int level, int window);

  /**
   * @brief 构建显示信息
   */
  virtual void BuildRepresentation() override;

protected:
  MPRSliceRepresentation();
  ~MPRSliceRepresentation() override;

  void updateInternal();

protected:
  int mLevel;
  int mWindow;
  vtkSmartPointer<vtkMatrix4x4> mResliceAxes;
  vtkSmartPointer<vtkImageAlgorithm> mReslicer;
  vtkSmartPointer<vtkImageData> mImageData;

  vtkSmartPointer<SliceNavigator> mSlicedNavigator;
  StandardPlane mPlaneType;

  ///@{
  /// 纹理映射 HU -> RGBA
  vtkSmartPointer<vtkTexture> mTexture;
  vtkSmartPointer<vtkImageMapToColors> mColorMap;
  vtkSmartPointer<vtkLookupTable> mLookupTable;
  ///@}
  
  /// @{
  /// 图像显示pipeline
  vtkSmartPointer<vtkPlaneSource> mPlaneSource;
  vtkSmartPointer<vtkActor> mTexturedActor;
  vtkSmartPointer<vtkPolyDataMapper> mTexturedMapper;
  ///@}
};
#endif // MPR_SLICE_REPRESENTATION_H
