#ifndef STUDY_INFORMATION_CXX_H
#define STUDY_INFORMATION_CXX_H
#include <QString>

struct StudyInformation
{
    /**
     * @brief 全球唯一标识
     */
    QString instanceUID;
    /**
     * @brief 检查日期
     */
    QString date;
    /**
     * @brief 检查时间
     */
    QString time;
    /**
     * @brief 检查描述
     */
    QString description;
    /**
     * @brief 检查标识（在一定范围内唯一）
     */
    QString id;
};
#endif // STUDY_INFORMATION_CXX_H