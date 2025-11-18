#ifndef EQUIPMENT_INFORMATION_CXX_H
#define EQUIPMENT_INFORMATION_CXX_H
#include "InformationParser.h"
#include "Macros.h"
#include <string>

class EquipmentInformation : public InformationParser
{
public:
  itkTypeMacro(EquipmentInformation, InformationParser);
  itkSetPointerDeclare(EquipmentInformation);
  itkFactorylessNewMacro(Self);
  ITK_DISALLOW_COPY_AND_MOVE(Self);

  itkSetStringMacro(Manufacturer);
  itkGetConstMacro(Manufacturer, std::string);

  itkSetStringMacro(ModelName);
  itkGetConstMacro(ModelName, std::string);

  itkSetStringMacro(SliceThickness);
  itkGetConstMacro(SliceThickness, std::string);

  itkSetStringMacro(SpacingBetweenSlices);
  itkGetConstMacro(SpacingBetweenSlices, std::string);

  itkSetStringMacro(ProtocolName);
  itkGetConstMacro(ProtocolName, std::string);

  itkSetStringMacro(EchoTime);
  itkGetConstMacro(EchoTime, std::string);

  itkSetStringMacro(RepetitionTime);
  itkGetConstMacro(RepetitionTime, std::string);

  itkSetStringMacro(XRayTubeCurrent);
  itkGetConstMacro(XRayTubeCurrent, std::string);
  void parseInfo(const itk::MetaDataDictionary& metaData) override;

protected:
  EquipmentInformation() = default;
  ~EquipmentInformation() override = default;

private:
  /**
   * @brief 制造商
   */
  std::string m_Manufacturer;
  /**
   * @brief 设备型号
   */
  std::string m_ModelName;
  /**
   * @brief 层厚(CT/MR)
   */
  std::string m_SliceThickness;
  /**
   * @brief 层间距
   */
  std::string m_SpacingBetweenSlices;
  /**
   * @brief 扫描协议名称
   */
  std::string m_ProtocolName;
  /**
   * @brief 回波时间(MRI)
   */
  std::string m_EchoTime;
  /**
   * @brief 重复时间(MRI)
   */
  std::string m_RepetitionTime;
  /**
   * @brief x射线管电流(mA)
   */
  std::string m_XRayTubeCurrent;
};
#endif // EQUIPMENT_INFORMATION_CXX_H