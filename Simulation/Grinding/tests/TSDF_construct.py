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


binary_volume = sitk.ReadImage(r"F:\Workspace\Projects\OViewer\Grinding\tests\mask_tooth_crop.nii.gz")
tsdf_image = binary_to_tsdf(binary_volume)
tsdf_image = np.transpose(tsdf_image, (2, 1, 0))
vtk_image = vtk.vtkImageData()
vtk_image.SetDimensions(tsdf_image.shape)
vtk_image.SetSpacing(binary_volume.GetSpacing())
vtk_image.SetOrigin(binary_volume.GetOrigin())

# 转换为 VTK 数组
vtk_array = numpy_support.numpy_to_vtk(tsdf_image.ravel(order='F'), deep=True, array_type=vtk.VTK_FLOAT)
vtk_image.GetPointData().SetScalars(vtk_array)

resized_vtk_image_filter = vtk.vtkImageReslice()
resized_vtk_image_filter.SetInputData(vtk_image)
resized_vtk_image_filter.SetOutputSpacing(binary_volume.GetSpacing()[0] * 0.5, binary_volume.GetSpacing()[1] * 0.5, binary_volume.GetSpacing()[2] * 0.5)
resized_vtk_image_filter.SetOutputExtent(0, tsdf_image.shape[0] * 2 - 1, 0, tsdf_image.shape[1] * 2 - 1, 0, tsdf_image.shape[2] * 2 - 1)
resized_vtk_image_filter.SetInterpolationModeToLinear()
resized_vtk_image_filter.Update()
# 2. 创建 Volume Mapper（GPU Ray Casting）
volume_mapper = vtk.vtkGPUVolumeRayCastMapper()
volume_mapper.SetInputData(resized_vtk_image_filter.GetOutput())
volume_mapper.SetBlendModeToComposite()  # 或 SetBlendModeToMaximumIntensity()

# 3. 创建 Transfer Functions（颜色 & 不透明度）
color_tf = vtk.vtkColorTransferFunction()
opacity_tf = vtk.vtkPiecewiseFunction()

# 关键：在 TSDF=0 处设置高不透明度（等值面）
iso_value = 0.0
surface_opacity = 1.0
color_tf.AddRGBPoint(iso_value, 0.8, 0.8, 1.0)  # 蓝色
opacity_tf.AddPoint(iso_value - 0.1, 0.0)
opacity_tf.AddPoint(iso_value, surface_opacity)
opacity_tf.AddPoint(iso_value + 0.1, 0.0)

# 4. Volume Property
volume_property = vtk.vtkVolumeProperty()
volume_property.SetColor(color_tf)
volume_property.SetScalarOpacity(opacity_tf)
volume_property.ShadeOn()          # 启用 Phong 光照
volume_property.SetInterpolationTypeToLinear()

# 5. Volume Actor
volume = vtk.vtkVolume()
volume.SetMapper(volume_mapper)
volume.SetProperty(volume_property)

# 6. Renderer
renderer = vtk.vtkRenderer()
render_window = vtk.vtkRenderWindow()
render_window.AddRenderer(renderer)
render_window_interactor = vtk.vtkRenderWindowInteractor()
render_window_interactor.SetRenderWindow(render_window)

renderer.AddVolume(volume)
renderer.SetBackground(0.1, 0.1, 0.1)

# 启用深度测试（重要！）
render_window.SetMultiSamples(0)  # 提升性能
renderer.UseDepthPeelingOn()

# 7. 启动交互
render_window.Render()
render_window_interactor.Start()
