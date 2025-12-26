import SimpleITK as sitk
import math
import numpy as np
import vtk
from vtkmodules.util import numpy_support


def sitk_to_vtk_image(sitk_image):
    """
    将SimpleITK图像转换为vtkImageData
    """
    # 获取numpy数组
    np_array = sitk.GetArrayFromImage(sitk_image)

    # 获取图像信息
    origin = sitk_image.GetOrigin()
    spacing = sitk_image.GetSpacing()
    direction = sitk_image.GetDirection()

    # 确定数据类型
    dtype = np_array.dtype
    if dtype == np.uint8:
        vtk_type = vtk.VTK_UNSIGNED_CHAR
    elif dtype == np.int16:
        vtk_type = vtk.VTK_SHORT
    elif dtype == np.uint16:
        vtk_type = vtk.VTK_UNSIGNED_SHORT
    elif dtype == np.int32:
        vtk_type = vtk.VTK_INT
    elif dtype == np.float32:
        vtk_type = vtk.VTK_FLOAT
    elif dtype == np.float64:
        vtk_type = vtk.VTK_DOUBLE
    else:
        raise ValueError(f"不支持的数据类型: {dtype}")

    # 将numpy数组转换为vtkImageData
    vtk_array = numpy_support.numpy_to_vtk(
        np_array.ravel(order='F'),  # Fortran顺序
        deep=True,
        array_type=vtk_type
    )

    # 创建vtkImageData
    vtk_image = vtk.vtkImageData()
    vtk_image.SetDimensions(sitk_image.GetSize())
    vtk_image.SetSpacing(spacing)
    vtk_image.SetOrigin(origin)

    # 设置方向矩阵（如果方向不是单位矩阵）
    if direction != (1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0):
        # 创建方向矩阵
        vtk_matrix = vtk.vtkMatrix4x4()
        for i in range(3):
            for j in range(3):
                vtk_matrix.SetElement(i, j, direction[i * 3 + j])
        vtk_image.SetDirectionMatrix(vtk_matrix)

    # 设置像素数据
    vtk_image.GetPointData().SetScalars(vtk_array)

    return vtk_image


def binary_to_tsdf(volume: sitk.Image, truncation_voxels=5):
    sdf_filter = sitk.SignedMaurerDistanceMapImageFilter()
    sdf_filter.InsideIsPositiveOn()
    sdf_filter.SquaredDistanceOff()
    sdf_filter.UseImageSpacingOn()
    sdf_volume = sdf_filter.Execute(volume)

    sdf = sitk.GetArrayFromImage(sdf_volume)
    tsdf = np.clip(sdf, -truncation_voxels, truncation_voxels)

    return tsdf.astype(np.float32)


def get_image_bounds(image):
    """
    获取SimpleITK图像的边界
    返回: [x_min, x_max, y_min, y_max, z_min, z_max]
    """
    if not isinstance(image, sitk.Image):
        raise TypeError("输入必须是SimpleITK.Image对象")

    # 获取图像的基本信息
    size = image.GetSize()  # 尺寸 (x, y, z)
    origin = image.GetOrigin()  # 原点 (x, y, z)
    spacing = image.GetSpacing()  # 间距 (x, y, z)

    # 计算每个维度的最小和最大坐标
    # 注意：原点对应体素的中心
    bounds = []

    for dim in range(image.GetDimension()):
        # 最小坐标 = 原点[dim] - 间距[dim]/2
        min_coord = origin[dim] - spacing[dim] / 2

        # 最大坐标 = 原点[dim] + (尺寸[dim]-0.5)*间距[dim]
        max_coord = origin[dim] + (size[dim] - 0.5) * spacing[dim]

        bounds.extend([min_coord, max_coord])

    return bounds


binary_volume = sitk.ReadImage(r"D:\Workspace\gitProject\build\bin\Release\data\1\coarsePredict\17"
                                   r"\mask_tooth_crop.nii.gz")
tsdf_image = binary_to_tsdf(binary_volume)
tsdf_image = np.transpose(tsdf_image, (2, 1, 0))
bounds = get_image_bounds(binary_volume)
