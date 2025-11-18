#ifndef PATIENT_CXX_H
#define PATIENT_CXX_H
#include "Macros.h"

#include <itkObject.h>
#include <string>

class PatientInformation : public itk::Object
{
public:
  itkTypeMacro(PatientInformation, itk::Object);
  itkSetPointerDeclare(PatientInformation);
  itkFactorylessNewMacro(Self);
  ITK_DISALLOW_COPY_AND_MOVE(Self);

  itkSetStringMacro(Name);
  itkGetConstMacro(Name, std::string);

  itkSetStringMacro(Id);
  itkGetConstMacro(Id, std::string);

  itkSetStringMacro(BirthDay);
  itkGetConstMacro(BirthDay, std::string);

  itkSetStringMacro(Sex);
  itkGetConstMacro(Sex, std::string);

  itkSetStringMacro(Age);
  itkGetConstMacro(Age, std::string);

  itkSetStringMacro(Size);
  itkGetConstMacro(Size, std::string);

  itkSetStringMacro(Weight);
  itkGetConstMacro(Weight, std::string);

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