#ifndef PATIENT_CXX_H
#define PATIENT_CXX_H
#include "Macros.h"

#include "InformationParser.h"
#include <string>
#include "DicomExport.h"

namespace itk
{
class MetaDataDictionary;
}
class DICOM_API PatientInformation : public InformationParser
{
public:
  itkTypeMacro(PatientInformation, InformationParser);
  itkSetPointerDeclare(PatientInformation);
  itkFactorylessNewMacro(Self);
  ITK_DISALLOW_COPY_AND_MOVE(Self);

  itkGetConstMacro(Name, std::string);

  itkGetConstMacro(Id, std::string);

  itkGetConstMacro(BirthDay, std::string);

  itkGetConstMacro(Sex, std::string);

  itkGetConstMacro(Age, std::string);

  itkGetConstMacro(Size, std::string);

  itkGetConstMacro(Weight, std::string);

  void parseInfo(const itk::MetaDataDictionary& metaData) override;

protected:
  PatientInformation() {}
  ~PatientInformation() override {}

private:
  /**
   * @brief 姓名
   */
  std::string m_Name;
  /**
   * @brief id
   */
  std::string m_Id;
  /**
   * @brief 出生日期
   */
  std::string m_BirthDay;
  /**
   * @brief 性别
   */
  std::string m_Sex;
  /**
   * @brief 年龄
   */
  std::string m_Age;
  /**
   * @brief 身高
   */
  std::string m_Size;
  /**
   * @brief 体重
   */
  std::string m_Weight;
};

#endif // PATIENT_CXX_H