/*****************************************************************/
/**
 * \file CubeOrientationMarker.h
 * \brief
 *3D视图下的正方体指示器\n
 * 使用方式如下：
 * \li SetNamedColor (optional)
 * \li SetPlaceFaces (optional)
 * \li SetScale (optional)
 * \li SetTextColor (optional)
 * \li SetTextLineWidth
 *(optional) \li Build
 *
 *
 * \author
 * \date   March 2024
 *********************************************************************/
#ifndef CUBE_ORIENTATION_MARKER
#define CUBE_ORIENTATION_MARKER
#include <string>
#include <unordered_map>
#include <vector>
#include <vtkActor.h>
#include <vtkAnnotatedCubeActor.h>
#include <vtkNamedColors.h>
#include <vtkPolyDataMapper.h>
#include <vtkSmartPointer.h>

class vtkOrientationMarkerWidget;
class vtkRenderWindowInteractor;
/**
 * @brief 3D视图下的正方体指示器.
 */
class CubeOrientationMarkerBuilder
{
public:
  /// @brief 指示器六个面的描述信息
  enum class PLANE_DESCRIPTION
  {
    /// @brief x轴阳面
    X_PLUS,
    /// @brief x轴阴面
    X_MINUS,
    /// @brief y轴阳面
    Y_PLUS,
    /// @brief y轴阴面
    Y_MINUS,
    /// @brief z轴阳面
    Z_PLUS,
    /// @brief z轴阴面
    Z_MINUS
  };

public:
  CubeOrientationMarkerBuilder(vtkRenderWindowInteractor* interactor);
  /// @brief 设置六个面的颜色值
  /// @param mapColor 对应面的颜色值 default (0.0, 0.0, 0.0)
  void SetNamedColor(std::unordered_map<PLANE_DESCRIPTION, std::array<float, 3>>& mapColor);
  /// @brief 设置六个面显示的内容
  /// @param mapText 对应面上显示的内容 default (X_Plus, X_Minus, Y_Plus, Y_Minus, Z_Plus,
  /// Z_Minus)
  void SetPlaneFaces(std::unordered_map<PLANE_DESCRIPTION, std::string>& mapText);
  /// @brief 设置显示文字的缩放系数
  /// @param scale 缩放系数 default 0.3
  void SetTextScale(float scale);
  /// @brief 设置文字颜色
  /// @param color 文字颜色 deault (1.0, 1.0, 1.0)
  void SetTextColor(float* color);
  /// @brief 设置文字宽度
  /// @param width 文字宽度 default 1.0
  void SetTextLineWidth(float width);
  /// @brief 构建方向指示器
  vtkSmartPointer<vtkOrientationMarkerWidget> Build();
  virtual ~CubeOrientationMarkerBuilder() = default;

protected:
  /// @brief 六个面对应颜色的映射
  std::unordered_map<PLANE_DESCRIPTION, std::array<float, 3>> mMapColor;
  /// @brief 六个面对应显示的文字
  std::unordered_map<PLANE_DESCRIPTION, std::string> mMapText;
  /// @brief 缩放系数
  float mScale;
  /// @brief 文字颜色 default (1.0, 1.0, 1.0)
  float mTextColor[3];
  /// @brief 文字宽度 default 1.0
  float mTextLineWidth;
  /// @brief 交互器
  vtkRenderWindowInteractor* mInteractor;
};

#endif // !CUBE_ORIENTATION_MARKER
