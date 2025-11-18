#ifndef DICOM_TAGS_H
#define DICOM_TAGS_H
namespace DicomTags
{
// 患者信息（Patient Information）
constexpr auto PatientName = "0010|0010";
constexpr auto PatientID = "0010|0020";
constexpr auto PatientBirthDate = "0010|0030";
constexpr auto PatientSex = "0010|0040";
constexpr auto PatientAge = "0010|1010";
constexpr auto PatientSize = "0010|1020";
constexpr auto PatientWeight = "0010|1030";

// 检查信息（Study Information）
constexpr auto StudyInstanceUID = "0020|000D";
constexpr auto StudyDate = "0008|0020";
constexpr auto StudyTime = "0008|0030";
constexpr auto StudyDescription = "0008|1030";
constexpr auto StudyID = "0020|0010";

// 序列信息（Series Information）
constexpr auto SeriesInstanceUID = "0020|000E";
constexpr auto Modality = "0008|0060";
constexpr auto SeriesDescription = "0008|103E";
constexpr auto SeriesNumber = "0020|0011";

// 图像信息（Image Information）
constexpr auto SOPInstanceUID = "0008|0018";
constexpr auto InstanceNumber = "0020|0013";
constexpr auto Rows = "0028|0010";
constexpr auto Columns = "0028|0011";
constexpr auto PixelSpacing = "0028|0030";
constexpr auto BitsAllocated = "0028|0100";
constexpr auto PixelRepresentation = "0028|0103";
constexpr auto PixelData = "7FE0|0010";

// 设备与采集参数（Equipment & Acquisition Parameters）
constexpr auto Manufacturer = "0008|0070";
constexpr auto ManufacturerModelName = "0008|1090";
constexpr auto SliceThickness = "0018|0050";
constexpr auto SpacingBetweenSlices = "0018|0088";
constexpr auto ProtocolName = "0018|1030";
constexpr auto EchoTime = "0018|0081";
constexpr auto RepetitionTime = "0018|0080";
constexpr auto XRayTubeCurrent = "0018|1151";

// 其他重要字段
constexpr auto ImageType = "0008|0008";
constexpr auto WindowCenter = "0028|1050";
constexpr auto WindowWidth = "0028|1051";
constexpr auto SamplesPerPixel = "0028|0002";
constexpr auto PhotometricInterpretation = "0028|0004";
}
#endif // DICOM_TAGS_H
