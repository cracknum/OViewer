#ifndef IMAGE_INFORMATION_CXX_H
#define IMAGE_INFORMATION_CXX_H
#include <QString>

struct ImageInformation
{
    /**
     * @brief 全球唯一标识
     */
    QString instanceUID;
    /**
     * @brief 图像序列中的编号
     */
    QString instanceNumber;
    /**
     * @brief 图像行数
     */
    QString rows;
    /**
     * @brief 图像列数
     */
    QString columns;
    /**
     * @brief 行像素spacing
     */
    QString rowPixelSpacing;
    /**
     * @brief 列像素spacing
     */
    QString columnPixelSpacing;
    /**
     * @brief 像素所占位数
     */
    QString bitsAllocated;
    /**
     * @brief 像素是否存在符号(0=无符号，1=有符号)
     */
    QString pixelRepresentation;
    /**
     * @brief [多值：像素的来源|图像在检查中的角色可选的附加信息]
     */
    QString imageType;
    /**
     * @brief 窗位
     */
    QString windowCenter;
    /**
     * @brief 窗宽
     */
    QString windowWidth;
    /**
     * @brief 每个像素表示的分量个数，结合photoMetricInterpretation解释图像
     * 
     */
    QString samplesPerPixel;
    /**
     * @brief 光度解释，就是对samplsPerPixel字段的分量的解释，例如如果这个值为RGB则sampersPerPixel三通道就分别表示RGB
     */
    QString photoMetricInterpretation;
    /**
     * @brief 实际存储数据
     */
    void* data;
};

#endif // IMAGE_INFORMATION_CXX_H