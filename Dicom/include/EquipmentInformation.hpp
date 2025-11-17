#ifndef EQUIPMENT_INFORMATION_CXX_H
#define EQUIPMENT_INFORMATION_CXX_H
#include <QString>

struct EquipmentInformation
{
    /**
     * @brief 制造商
     */
    QString manufacturer;
    /**
     * @brief 设备型号
     */
    QString modelName;
    /**
     * @brief 层厚(CT/MR)
     */
    QString sliceThickness;
    /**
     * @brief 层间距
     */
    QString spacingBetweenSlices;
    /**
     * @brief 扫描协议名称
     */
    QString protocalName;
    /**
     * @brief 回波时间(MRI)
     */
    QString echoTime;
    /**
     * @brief 重复时间(MRI)
     */
    QString repetitionTime;
    /**
     * @brief x射线管电流(mA)
     */
    QString xRayTubeCurrent;
};
#endif // EQUIPMENT_INFORMATION_CXX_H