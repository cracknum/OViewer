#ifndef SERIES_INFORMATION_CXX_H
#define SERIES_INFORMATION_CXX_H
#include <QString>

struct SeriesInformation
{
    /**
     * @brief 全球唯一标识
     */
    QString instanceUID;
    /**
     * @brief 模态（CT|MR|US|DX etc.）
     */
    QString modality;
    /**
     * @brief 检查描述
     */
    QString description;
    /**
     * @brief 在一次检查中的编号
     */
    QString number;
};
#endif // SERIES_INFORMATION_CXX_H