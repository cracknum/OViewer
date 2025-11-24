#ifndef IMAGE_INFORMATION_CXX_H
#define IMAGE_INFORMATION_CXX_H
#include "Macros.h"

#include "InformationParser.h"
#include <itkImage.h>
#include <string>
#include "DicomExport.h"
#include <any>
#include <itkImageRegion.h>

class DICOM_API ImageInformation final : public InformationParser
{
public:
  itkTypeMacro(ImageInformation, InformationParser);
  itkSetPointerDeclare(ImageInformation);
  itkFactorylessNewMacro(Self);
  ITK_DISALLOW_COPY_AND_MOVE(Self);

  itkGetConstMacro(InstanceUID, std::string);

  itkGetConstMacro(Rows, std::string);

  itkGetConstMacro(Columns, std::string);

  itkGetConstMacro(RowPixelSpacing, std::string);

  itkGetConstMacro(ColumnPixelSpacing, std::string);

  itkGetConstMacro(BitsAllocated, std::string);

  itkGetConstMacro(PixelRepresentation, std::string);

  itkGetConstMacro(ImageType, std::string);

  itkGetConstMacro(WindowCenter, std::string);

  itkGetConstMacro(WindowWidth, std::string);

  itkGetConstMacro(SamplesPerPixel, std::string);

  itkGetConstMacro(PhotoMetricInterpretation, std::string);

  itkGetConstMacro(Volume, itk::Object::Pointer);
  itkGetConstMacro(Dimensions, itk::Size<3>);
  void parseInfo(const itk::MetaDataDictionary& metaData) override;

  template <typename TPixel, typename ImageType = itk::Image<TPixel, 3>>
  void SetVolume(typename ImageType::Pointer image)
  {
    m_Origin = image->GetOrigin();
    m_Direction = image->GetDirection();
    m_Spacing = image->GetSpacing();
    m_Volume = image;
    m_Dimensions = image->GetLargestPossibleRegion().GetSize();
  }

protected:
  ImageInformation();
  ~ImageInformation() override = default;
  void PrintSelf(std::ostream & os, itk::Indent indent) const override;
private:
  /**
   * @brief 全球唯一标识
   */
  std::string m_InstanceUID;
  /**
   * @brief 图像行数
   */
  std::string m_Rows;
  /**
   * @brief 图像列数
   */
  std::string m_Columns;
  /**
   * @brief 行像素spacing
   */
  std::string m_RowPixelSpacing;
  /**
   * @brief 列像素spacing
   */
  std::string m_ColumnPixelSpacing;
  /**
   * @brief 像素所占位数
   */
  std::string m_BitsAllocated;
  /**
   * @brief 像素是否存在符号(0=无符号，1=有符号)
   */
  std::string m_PixelRepresentation;
  /**
   * @brief [多值：像素的来源|图像在检查中的角色可选的附加信息]
   */
  std::string m_ImageType;
  /**
   * @brief 窗位
   */
  std::string m_WindowCenter;
  /**
   * @brief 窗宽
   */
  std::string m_WindowWidth;
  /**
   * @brief 每个像素表示的分量个数，结合photoMetricInterpretation解释图像
   *
   */
  std::string m_SamplesPerPixel;
  /**
   * @brief
   * 光度解释，就是对samplsPerPixel字段的分量的解释，例如如果这个值为RGB则sampersPerPixel三通道就分别表示RGB
   */
  std::string m_PhotoMetricInterpretation;
  /**
   * @brief 实际存储数据
   */
  itk::Object::Pointer m_Volume;

  /**
   * @brief 体数据的spacing
   */
  itk::Vector<double> m_Spacing;
  /**
   * @brief 体数据的原点
   */
  itk::Point<double> m_Origin;
  /**
   * @brief 体数据的方向矩阵，注意，这是方向矩阵，并不包含缩放，缩放由对角线的spacing提供
   */
  itk::Matrix<double> m_Direction;
  itk::Size<3> m_Dimensions;
};

#endif // IMAGE_INFORMATION_CXX_H