#ifndef SERIES_INFORMATION_CXX_H
#define SERIES_INFORMATION_CXX_H
#include "Macros.h"

#include <itkObject.h>
#include <string>
#include "DicomExport.h"
#include "InformationParser.h"

class DICOM_API SeriesInformation : public InformationParser
{
public:
  itkTypeMacro(SeriesInformation, InformationParser);
  itkSetPointerDeclare(SeriesInformation);
  itkFactorylessNewMacro(Self);
  ITK_DISALLOW_COPY_AND_MOVE(Self);

  itkSetStringMacro(InstanceUID);
  itkGetConstMacro(InstanceUID, std::string);

  itkSetStringMacro(Modality);
  itkGetConstMacro(Modality, std::string);

  itkSetStringMacro(Description);
  itkGetConstMacro(Description, std::string);

  itkSetStringMacro(Number);
  itkGetConstMacro(Number, std::string);

  void parseInfo(const itk::MetaDataDictionary& metaData) override;

protected:
  SeriesInformation() {}
  ~SeriesInformation() override {}

private:
  /**
   * @brief 全球唯一标识
   */
  std::string m_InstanceUID;
  /**
   * @brief 模态（CT|MR|US|DX etc.）
   */
  std::string m_Modality;
  /**
   * @brief 检查描述
   */
  std::string m_Description;
  /**
   * @brief 在一次检查中的编号
   */
  std::string m_Number;
};
#endif // SERIES_INFORMATION_CXX_H