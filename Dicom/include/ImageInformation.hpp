#ifndef IMAGE_INFORMATION_CXX_H
#define IMAGE_INFORMATION_CXX_H
#include "Macros.h"

#include <string>
#include "InformationParser.h"

class ImageInformation final : public InformationParser
{
public:
  itkTypeMacro(ImageInformation, InformationParser);
  itkSetPointerDeclare(ImageInformation);
  itkFactorylessNewMacro(Self);
  ITK_DISALLOW_COPY_AND_MOVE(Self);

  itkSetStringMacro(InstanceUID);
  itkGetConstMacro(InstanceUID, std::string);

  itkSetStringMacro(Rows);
  itkGetConstMacro(Rows, std::string);

  itkSetStringMacro(Columns);
  itkGetConstMacro(Columns, std::string);

  itkSetStringMacro(RowPixelSpacing);
  itkGetConstMacro(RowPixelSpacing, std::string);

  itkSetStringMacro(ColumnPixelSpacing);
  itkGetConstMacro(ColumnPixelSpacing, std::string);

  itkSetStringMacro(BitsAllocated);
  itkGetConstMacro(BitsAllocated, std::string);

  itkSetStringMacro(PixelRepresentation);
  itkGetConstMacro(PixelRepresentation, std::string);

  itkSetStringMacro(ImageType);
  itkGetConstMacro(ImageType, std::string);

  itkSetStringMacro(WindowCenter);
  itkGetConstMacro(WindowCenter, std::string);

  itkSetStringMacro(WindowWidth);
  itkGetConstMacro(WindowWidth, std::string);

  itkSetStringMacro(SamplesPerPixel);
  itkGetConstMacro(SamplesPerPixel, std::string);

  itkSetStringMacro(PhotoMetricInterpretation);
  itkGetConstMacro(PhotoMetricInterpretation, std::string);

  itkSetMacro(Volume, void*);
  itkGetConstMacro(Volume, void*);
  void parseInfo(const itk::MetaDataDictionary& metaData) override;

protected:
  ImageInformation();
  ~ImageInformation() override = default;

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
  void* m_Volume;
};

#endif // IMAGE_INFORMATION_CXX_H