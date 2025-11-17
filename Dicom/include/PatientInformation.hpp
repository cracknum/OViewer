#ifndef PATIENT_CXX_H
#define PATIENT_CXX_H
#include <qstring.h>

struct PatientInformation
{
    /**
     * @brief 姓名
     */
    QString name;
    /**
     * @brief id
     */
    QString id;
    /**
     * @brief 出生日期
     */
    QString birthDay;
    /**
     * @brief 性别
     */
    QString sex;
    /**
     * @brief 年龄
     */
    QString age;
    /**
     * @brief 身高
     */
    QString size;
    /**
     * @brief 体重
     */
    QString weight;
};

#endif // PATIENT_CXX_H