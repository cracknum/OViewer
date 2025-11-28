# 基于VTK的MPR实现


## Radiologist's view和Neuro-Surgeons's view

|视角|	观察方向	|图像朝向约定	|典型用途|
|---|------------|---------------|--------|
|Radiologist's view|	从患者脚侧往头侧看（即“从下往上看”）<br>（例如轴位图像是从足端向上看）|	图像显示为 患者面对你 <br>→ 左侧显示患者的右侧（R）<br>→ 右侧显示患者的左侧（L）|	放射诊断、阅片（PACS 系统默认）|
|Neuro-Surgeon's view	|从患者头侧往脚侧看（即“从上往下看”）<br>（例如站在患者头顶向下看）	|图像显示为 你面对患者 <br>→ 左侧显示患者的左侧（L）<br>→ 右侧显示患者的右侧（R）	|手术规划、导航、术中参考|

Radiologist's view为默认视角，也是DICOM中存储的默认视角LPS坐标系（这个坐标系不会用于可视化，即使使用左手系也是LAS），但是显示一般使用RAS坐标系，LAS和RAS就是x轴不同其他两个轴是相同的，也就是关于x平面镜像

## mitk中对于Geometry的管理
- SliceNavigationController
    - m_InputWorldTimeGeometry： 整体由mitkDataStorage计算得到的整体的geometry信息
    - slicedWorldGeometry 
    ```c++
    void SliceNavigationController::CreateWorldGeometry(bool top, bool frontside, bool rotated)
  {
    ...

    // initialize the viewplane
    SlicedGeometry3D::Pointer slicedWorldGeometry;
    BaseGeometry::ConstPointer currentGeometry;
    ...
    currentGeometry = m_InputWorldTimeGeometry->GetGeometryForTimeStep(selectedTimestep);
    ...
    if (AnatomicalPlane::Original == m_ViewDirection)
    {
      slicedWorldGeometry = dynamic_cast<SlicedGeometry3D*>(
        m_InputWorldTimeGeometry->GetGeometryForTimeStep(selectedTimestep).GetPointer());
      if (slicedWorldGeometry.IsNull())
      {
        slicedWorldGeometry = SlicedGeometry3D::New();
        slicedWorldGeometry->InitializePlanes(currentGeometry, AnatomicalPlane::Original, top, frontside, rotated);
        slicedWorldGeometry->SetSliceNavigationController(this);
      }
    }
    ...

    m_CreatedWorldGeometry = mitk::TimeGeometry::Pointer();
    if (nullptr != dynamic_cast<const mitk::ProportionalTimeGeometry*>(m_InputWorldTimeGeometry.GetPointer()))
    {
      const TimePointType minimumTimePoint = m_InputWorldTimeGeometry->TimeStepToTimePoint(0);
      const TimePointType stepDuration =
        m_InputWorldTimeGeometry->TimeStepToTimePoint(1) - minimumTimePoint;

      auto createdTimeGeometry = ProportionalTimeGeometry::New();
      createdTimeGeometry->Initialize(slicedWorldGeometry, inputTimeSteps);
      createdTimeGeometry->SetFirstTimePoint(minimumTimePoint);
      createdTimeGeometry->SetStepDuration(stepDuration);

      m_CreatedWorldGeometry = createdTimeGeometry;
    }
    ...
  }

  void SliceNavigationController::SendCreatedWorldGeometry()
  {
    if (!m_BlockUpdate)
    {
      this->InvokeEvent(GeometrySendEvent(m_CreatedWorldGeometry, 0));
    }
  }

  // 通过mitk::SliceNavigationController来设置geometry
  void mitk::BaseRenderer::SetGeometry(const itk::EventObject& geometrySendEvent)
    {
    const auto* sendEvent = dynamic_cast<const SliceNavigationController::GeometrySendEvent*>(&geometrySendEvent);

    if (nullptr == sendEvent)
    {
        return;
    }

    SetWorldTimeGeometry(sendEvent->GetTimeGeometry());
    }

    void mitk::BaseRenderer::SetWorldTimeGeometry(const mitk::TimeGeometry* geometry)
    {
        if (m_WorldTimeGeometry == geometry)
        {
            return;
        }

        m_WorldTimeGeometry = geometry;

        this->UpdateCurrentGeometries();
    }

    void mitk::BaseRenderer::UpdateCurrentGeometries()
    {
    ...

    auto slicedWorldGeometry =
        dynamic_cast<SlicedGeometry3D*>(m_WorldTimeGeometry->GetGeometryForTimeStep(m_TimeStep).GetPointer());
    if (slicedWorldGeometry != nullptr)
    {
        ...
        SetCurrentWorldGeometry(slicedWorldGeometry);
        SetCurrentWorldPlaneGeometry(slicedWorldGeometry->GetPlaneGeometry(m_Slice));
        ...
    }
    }
    ```