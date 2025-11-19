#ifndef STUDY_INFORMATION_CXX_H
#define STUDY_INFORMATION_CXX_H
#include "Macros.h"

#include <itkObject.h>
#include <string>
#include "DicomExport.h"
#include "InformationParser.h"

class DICOM_API StudyInformation : public InformationParser
{
public:
  itkTypeMacro(StudyInformation, InformationParser);
  itkSetPointerDeclare(StudyInformation);
  itkFactorylessNewMacro(Self);
  ITK_DISALLOW_COPY_AND_MOVE(Self);

  itkSetStringMacro(InstanceUID);
  itkGetConstMacro(InstanceUID, std::string);

  itkSetStringMacro(Date);
  itkGetConstMacro(Date, std::string);

  itkSetStringMacro(Time);
  itkGetConstMacro(Time, std::string);

  itkSetStringMacro(Description);
  itkGetConstMacro(Description, std::string);

  itkSetStringMacro(Id);
  itkGetConstMacro(Id, std::string);

  void parseInfo(const itk::MetaDataDictionary& metaData) override;

protected:
  StudyInformation() {}
  ~StudyInformation() override {}

private:
  /**
   * @brief 全球唯一标识
   */
  std::string m_InstanceUID;
  /**
   * @brief 检查日期
   */
  std::string m_Date;
  /**
   * @brief 检查时间
   */
  std::string m_Time;
  /**
   * @brief 检查描述
   */
  std::string m_Description;
  /**
   * @brief 检查标识（在一定范围内唯一）
   */
  std::string m_Id;
};
#endif // STUDY_INFORMATION_CXX_H