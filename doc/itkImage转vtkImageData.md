# itkImage转vtkImageData

虽然ITK官方仓库提供了`itkImageToVTKImageFilter`来将itkImage转换为vtkImage，但是没有考虑一个问题（或者更多问题）：原点对准
在itk中origin（世界坐标系下的原点）是第一个体素的center位置，而在vtk中的origin（世界坐标系下的原点）是第一个体素的corner位置，如果是单纯的显示或者切片并不会出现问题，但是如果用于配准就会出现相差0.5*spacing的大小,导致出现问题

```c++
vtkSmartPointer<vtkImageChangeInformation> changeOriginFilter = vtkSmartPointer<vtkImageChangeInformation>::New();
	changeOriginFilter->SetInputData(imageExportFilter->GetOutput());

	double newOrigin[3] = {m_Origin[0] - 0.5 * m_Spacing[0], m_Origin[1] - 0.5 * m_Spacing[1], m_Origin[2] - 0.5 * m_Spacing[2]};
	changeOriginFilter->SetInputData(m_VtkImage);
	changeOriginFilter->SetOutputOrigin(newOrigin);
	changeOriginFilter->Update();
	m_VtkImage = changeOriginFilter->GetOutput();
```

可能还会有更多的问题，目前无法解决