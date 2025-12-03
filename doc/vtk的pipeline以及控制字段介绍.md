# vtk的pipeline以及控制字段介绍

## 1 vtkExecutive与执行pipeline控制

在Filter或者说在vtkAlgorithm的子类的执行过程中并不知道我的上游是是谁，我只需要告诉我的上级我需要什么，告诉我的下级我能够产出什么，但是我不会直接知道我的上下级是谁，而这个上下级的数据请求和流动是由vtkExecutive及其子类vtkDemandDrivenPipeline来进行管理的。

当vtkAlgorithm调用Update方法时，由vtkExecutive来具体的执行整个流程
```c++
void vtkAlgorithm::Update()
{
  int port = -1;
  if (this->GetNumberOfOutputPorts())
  {
    port = 0;
  }
  this->Update(port);
}

void vtkAlgorithm::Update(int port)
{
  this->GetExecutive()->Update(port);
}

vtkTypeBool vtkDemandDrivenPipeline::Update(int port)
{
  if (!this->UpdateInformation())
  {
    return 0;
  }
  if (port >= -1 && port < this->Algorithm->GetNumberOfOutputPorts())
  {
    return this->UpdateData(port);
  }
  else
  {
    return 1;
  }
}
```

整个更新流程包括两部分UpdateInformation和UpdateData。

### 1.1.1 UpdateInformation
---

1. ✅ 整体目标

> **`UpdateInformation()` 的目的是：触发并完成 `REQUEST_INFORMATION` 阶段，让整个管线从源头到末端协商好“数据能提供什么”**（元数据）

这包括：
- 数据类型（`DATA_TYPE_NAME`）
- 全局范围（`WHOLE_EXTENT`）
- 时间步（`TIME_STEPS`）
- 间距、原点（`SPACING`, `ORIGIN`）
- 是否支持流式处理等

这些信息**不包含实际数据**，只描述“能力”或“结构”。

---

2. 🔍 逐行解析

```cpp
if (!this->CheckAlgorithm("UpdateInformation", nullptr))
{
  return 0;
}
```
- 确保当前 `Executive` 已绑定到一个有效的 `vtkAlgorithm`；
- 防止在未初始化状态下调用。

---

3. **先执行 `UpdateDataObject()`**
```cpp
if (!this->UpdateDataObject())
{
  return 0;
}
```
> ⚠️ 这是关键！很多人误以为 `REQUEST_INFORMATION` 是第一步，其实 **`REQUEST_DATA_OBJECT` 更早**。

- `UpdateDataObject()` 会触发 `REQUEST_DATA_OBJECT` 请求；
- 目的是为每个 filter 的输出端口 **创建空的 `vtkDataObject` 实例**（如 `vtkPolyData*`）；
- 只有存在输出对象后，才能在其 `vtkInformation` 中设置元数据（如 `WHOLE_EXTENT`）。

✅ **执行顺序**：
```
REQUEST_DATA_OBJECT → REQUEST_INFORMATION → REQUEST_UPDATE_EXTENT → REQUEST_DATA
```

---

4. 创建并配置 `InfoRequest`
```cpp
if (!this->InfoRequest)
{
  this->InfoRequest = vtkInformation::New();
  this->InfoRequest->Set(REQUEST_INFORMATION());
  // 请求方向：向上游传播（从 mapper → source）
  this->InfoRequest->Set(vtkExecutive::FORWARD_DIRECTION(), vtkExecutive::RequestUpstream);
  // 算法在请求转发“之后”才处理它（即先传到上游，再从上游开始处理）
  this->InfoRequest->Set(vtkExecutive::ALGORITHM_AFTER_FORWARD(), 1);
}
```

4.1 关键参数说明：

| 设置 | 作用 |
|------|------|
| `FORWARD_DIRECTION = RequestUpstream` | 表示该请求从**当前节点向数据源方向传播**（逆数据流） |
| `ALGORITHM_AFTER_FORWARD = 1` | 表示：**先将请求传递给所有上游，等到达最源头后，再从源头开始逐级处理**（深度优先） |

6. 发送请求
```cpp
return this->ProcessRequest(
  this->InfoRequest, this->GetInputInformation(), this->GetOutputInformation());
```
- 调用基类 `vtkExecutive::ProcessRequest()`；
- 启动整个 `REQUEST_INFORMATION` 的传播与处理流程；
- 返回 `0` 表示失败（如某个 filter 报错），`1` 表示成功。

---

7. 🔄 在完整管线中的位置

当用户调用：
```cpp
mapper->Update();  // 或 filter->Update()
```

VTK 内部执行顺序如下：

| 步骤 | 方法 | 请求类型 | 方向 |
|------|------|--------|------|
| 1 | `UpdateDataObject()` | `REQUEST_DATA_OBJECT` | Downstream（创建输出对象） |
| 2 | **`UpdateInformation()`** | **`REQUEST_INFORMATION`** | **Upstream**（协商元数据） |
| 3 | `UpdateExtent()` | `REQUEST_UPDATE_EXTENT` | Upstream（协商更新范围） |
| 4 | `UpdateData()` | `REQUEST_DATA` | Downstream（生成真实数据） |

> 因此，`UpdateInformation()` 是 **第二步**，但却是 **第一个“协商”阶段**。

---

8. 💡 为什么需要这个阶段？

假设没有 `REQUEST_INFORMATION`：
- 下游 filter 不知道输入数据的 `WHOLE_EXTENT`，无法正确设置裁剪范围；
- 时间序列 filter 不知道有哪些 `TIME_STEPS`，无法选择时间点；
- 多块数据 filter 不知道有多少块，无法分配内存。

通过此阶段，VTK 实现了 **“先了解全局结构，再决定局部计算”** 的智能管线。

---

### 1.1.2 UpdateData

对每个输出对象或者说output port执行一遍向上的数据请求，以获取当前filter的正确输入对象

```c++
int vtkDemandDrivenPipeline::UpdateData(int outputPort)
{
  // The algorithm should not invoke anything on the executive.
  if (!this->CheckAlgorithm("UpdateData", nullptr))
  {
    return 0;
  }

  // Range check.
  if (outputPort < -1 || outputPort >= this->Algorithm->GetNumberOfOutputPorts())
  {
    vtkErrorMacro("UpdateData given output port index " << outputPort << " on an algorithm with "
                                                        << this->Algorithm->GetNumberOfOutputPorts()
                                                        << " output ports.");
    return 0;
  }

  // Setup the request for data.
  if (!this->DataRequest)
  {
    this->DataRequest = vtkInformation::New();
    this->DataRequest->Set(REQUEST_DATA());
    // The request is forwarded upstream through the pipeline.
    this->DataRequest->Set(vtkExecutive::FORWARD_DIRECTION(), vtkExecutive::RequestUpstream);
    // Algorithms process this request after it is forwarded.
    this->DataRequest->Set(vtkExecutive::ALGORITHM_AFTER_FORWARD(), 1);
  }

  // Send the request.
  this->DataRequest->Set(FROM_OUTPUT_PORT(), outputPort);
  return this->ProcessRequest(
    this->DataRequest, this->GetInputInformation(), this->GetOutputInformation());
}
```
---

## 2 自定义vtk filter

在自定义filter时我们经常需要的几处设置为

1. 设置输入和输出端口数
```c++
  this->SetNumberOfInputPorts(1);
  this->SetNumberOfOutputPorts(1);
```
2. 设置输入和输出数据类型
```c++

int vtkPolyDataAlgorithm::FillOutputPortInformation(int vtkNotUsed(port), vtkInformation* info)
{
  // now add our info
  info->Set(vtkDataObject::DATA_TYPE_NAME(), "vtkPolyData");
  return 1;
}

int vtkPolyDataAlgorithm::FillInputPortInformation(int vtkNotUsed(port), vtkInformation* info)
{
  info->Set(vtkAlgorithm::INPUT_REQUIRED_DATA_TYPE(), "vtkPolyData");
  return 1;
}
```
因为`DATA_TYPE_NAME`属于是输出对象的属性所以设置到了vtkDataObject上，而`INPUT_REQUIRED_DATA_TYPE`是因为算法要求的输入类型，所以设置到了`vtkAlgorithm`上
4. 获取输入和数据
```c++
  // get the info objects
  vtkInformation* inInfo = inputVector[0]->GetInformationObject(0);
  vtkInformation* outInfo = outputVector->GetInformationObject(0);

  // get the input and output and check its validity
  vtkPolyData* input = vtkPolyData::SafeDownCast(inInfo->Get(vtkDataObject::DATA_OBJECT()));
  vtkPolyData* output = vtkPolyData::SafeDownCast(outInfo->Get(vtkDataObject::DATA_OBJECT()));
```

## 3 认识vtkDataObject

在vtkDataObject对象中定义了大量的类似于`DATA_OBJECT`和`DATA_TYPE_NAME`的大量的key，可用于在执行构成中获取上游已经设置的数据
```c++

  // \ingroup InformationKeys
  static vtkInformationStringKey* DATA_TYPE_NAME();
  // \ingroup InformationKeys
  static vtkInformationDataObjectKey* DATA_OBJECT();
  // \ingroup InformationKeys
  static vtkInformationIntegerKey* DATA_EXTENT_TYPE();
  // \ingroup InformationKeys
  static vtkInformationIntegerPointerKey* DATA_EXTENT();
  // \ingroup InformationKeys
  static vtkInformationIntegerVectorKey* ALL_PIECES_EXTENT();
  // \ingroup InformationKeys
  static vtkInformationIntegerKey* DATA_PIECE_NUMBER();
  // \ingroup InformationKeys
  static vtkInformationIntegerKey* DATA_NUMBER_OF_PIECES();
  // \ingroup InformationKeys
  static vtkInformationIntegerKey* DATA_NUMBER_OF_GHOST_LEVELS();
  // \ingroup InformationKeys
  static vtkInformationDoubleKey* DATA_TIME_STEP();
  // \ingroup InformationKeys
  static vtkInformationInformationVectorKey* POINT_DATA_VECTOR();
  // \ingroup InformationKeys
  static vtkInformationInformationVectorKey* CELL_DATA_VECTOR();
  // \ingroup InformationKeys
  static vtkInformationInformationVectorKey* VERTEX_DATA_VECTOR();
  // \ingroup InformationKeys
  static vtkInformationInformationVectorKey* EDGE_DATA_VECTOR();
  // \ingroup InformationKeys
  static vtkInformationIntegerKey* FIELD_ARRAY_TYPE();
  // \ingroup InformationKeys
  static vtkInformationIntegerKey* FIELD_ASSOCIATION();
  // \ingroup InformationKeys
  static vtkInformationIntegerKey* FIELD_ATTRIBUTE_TYPE();
  // \ingroup InformationKeys
  static vtkInformationIntegerKey* FIELD_ACTIVE_ATTRIBUTE();
  // \ingroup InformationKeys
  static vtkInformationIntegerKey* FIELD_NUMBER_OF_COMPONENTS();
  // \ingroup InformationKeys
  static vtkInformationIntegerKey* FIELD_NUMBER_OF_TUPLES();
  // \ingroup InformationKeys
  static vtkInformationIntegerKey* FIELD_OPERATION();
  // \ingroup InformationKeys
  static vtkInformationDoubleVectorKey* FIELD_RANGE();
  // \ingroup InformationKeys
  static vtkInformationIntegerVectorKey* PIECE_EXTENT();
  // \ingroup InformationKeys
  static vtkInformationStringKey* FIELD_NAME();
  // \ingroup InformationKeys
  static vtkInformationDoubleVectorKey* ORIGIN();
  // \ingroup InformationKeys
  static vtkInformationDoubleVectorKey* SPACING();
  // \ingroup InformationKeys
  static vtkInformationDoubleVectorKey* DIRECTION();
  // \ingroup InformationKeys
  static vtkInformationDoubleVectorKey* BOUNDING_BOX();

  // Key used to put SIL information in the output information by readers.
  // \ingroup InformationKeys
  static vtkInformationDataObjectKey* SIL();

```
### 3.1 核心概念：`vtkInformation` 与 Key-Value 机制

VTK 使用 `vtkInformation` 作为**通用的元数据容器**，其内部是一个 `Key → Value` 的映射表。  
每个 Key 是一个**类型安全的静态对象**（如 `vtkInformationStringKey*`），确保：
- 编译期类型检查；
- 避免字符串拼写错误；
- 支持复杂值类型（指针、向量、嵌套信息等）。

这些 Key 定义在 `vtkDataObject` 中，主要用于：
- **`REQUEST_INFORMATION` 阶段**：描述数据“能提供什么”；
- **`REQUEST_UPDATE_EXTENT` 阶段**：协商“需要计算哪部分”；
- **字段/属性管理**：描述标量、矢量、张量等数组信息。

---

### 3.2 按键功能分类详解

1. **数据对象基本身份**
| Key | 类型 | 说明 |
|-----|------|------|
| `DATA_TYPE_NAME()` | `StringKey` | 数据类型名称，如 `"vtkPolyData"`。**用于创建输出对象**（`REQUEST_DATA_OBJECT`）。 |
| `DATA_OBJECT()` | `DataObjectKey` | 指向实际的 `vtkDataObject*` 实例。**在 `REQUEST_DATA` 阶段填充真实数据后设置**。 |

> ✅ 这两个是最基础的：先声明“我是谁”（`DATA_TYPE_NAME`），再放“我自己”（`DATA_OBJECT`）。

---

2. **空间/时间范围与分块**（Extent & Time）
| Key | 类型 | 说明 |
|-----|------|------|
| `DATA_EXTENT_TYPE()` | `IntegerKey` | 范围类型（`VTK_3D_EXTENT`, `VTK_2D_EXTENT` 等） |
| `DATA_EXTENT()` | `IntegerPointerKey` | 全局数据范围（如图像的 `[imin, imax, jmin, jmax, kmin, kmax]`） |
| `ALL_PIECES_EXTENT()` | `IntegerVectorKey` | 所有分块的联合范围（用于并行/分布式） |
| `PIECE_EXTENT()` | `IntegerVectorKey` | 当前 piece（分块）的范围 |
| `DATA_PIECE_NUMBER()` | `IntegerKey` | 当前处理的是第几块（0-based） |
| `DATA_NUMBER_OF_PIECES()` | `IntegerKey` | 总共多少块 |
| `DATA_NUMBER_OF_GHOST_LEVELS()` | `IntegerKey` | 幽灵层（ghost levels）数量，用于并行边界同步 |
| `DATA_TIME_STEP()` | `DoubleKey` | 当前请求的时间步（用于时序数据） |

> 💡 这些键支撑了 VTK 的 **流式处理、并行计算、时序动画** 能力。

---

3. **几何与拓扑属性**
| Key | 类型 | 说明 |
|-----|------|------|
| `ORIGIN()` | `DoubleVectorKey` | 数据原点（如图像 `(x0, y0, z0)`） |
| `SPACING()` | `DoubleVectorKey` | 像素/体素间距（仅结构化数据） |
| `DIRECTION()` | `DoubleVectorKey` | 坐标系方向矩阵（9 元素，行优先） |
| `BOUNDING_BOX()` | `DoubleVectorKey` | 包围盒 `[xmin, xmax, ymin, ymax, zmin, zmax]` |

> ⚠️ 注意：`ORIGIN`/`SPACING` 主要用于 `vtkImageData`、`vtkRectilinearGrid` 等结构化数据。

---

4. **字段数据**（Field Data / Arrays）
VTK 将属性数据（标量、矢量、纹理坐标等）称为 **“字段”**（Field Data），按关联位置分类：

| 关联类型 | Key | 说明 |
|--------|-----|------|
| **点属性** | `POINT_DATA_VECTOR()` | 存储所有点属性数组的 `vtkInformationVector` |
| **单元属性** | `CELL_DATA_VECTOR()` | 存储所有单元属性数组的信息 |
| **顶点属性**（Graph） | `VERTEX_DATA_VECTOR()` | 图结构中的顶点属性 |
| **边属性**（Graph） | `EDGE_DATA_VECTOR()` | 图结构中的边属性 |

每个属性数组自身的信息通过以下 Key 描述（通常在 `vtkInformation` 子对象中）：

| 数组属性 Key | 类型 | 说明 |
|-------------|------|------|
| `FIELD_NAME()` | `StringKey` | 数组名称，如 `"Temperature"` |
| `FIELD_ASSOCIATION()` | `IntegerKey` | 关联类型（`FIELD_ASSOCIATION_POINTS`, `CELLS` 等） |
| `FIELD_ATTRIBUTE_TYPE()` | `IntegerKey` | 是否为活跃属性（`SCALARS`, `VECTORS`, `NORMALS`, `TENSORS`, `TCOORDS`） |
| `FIELD_ACTIVE_ATTRIBUTE()` | `IntegerKey` | 是否设为“当前活跃”（用于 mapper 自动选择） |
| `FIELD_NUMBER_OF_COMPONENTS()` | `IntegerKey` | 分量数（1=标量，3=矢量，9=张量） |
| `FIELD_NUMBER_OF_TUPLES()` | `IntegerKey` | 元组数（应等于点数或单元数） |
| `FIELD_RANGE()` | `DoubleVectorKey` | 数据范围 `[min, max]`（用于颜色映射） |
| `FIELD_ARRAY_TYPE()` | `IntegerKey` | 数据类型（`VTK_FLOAT`, `VTK_INT` 等） |
| `FIELD_OPERATION()` | `IntegerKey` | 字段操作（如 `VTK_FIELD_OPERATION_INSERT`） |

> ✅ 这些键使得 filter 可以：
> - 查询输入是否有 `"Normals"`；
> - 创建新的 `"Pressure"` 标量场；
> - 设置某数组为活跃矢量场供 glyph 使用。

---

5. **高级结构支持**
| Key | 类型 | 说明 |
|-----|------|------|
| `SIL()` | `DataObjectKey` | **Subset Inclusion Lattice**（子集包含格），用于复杂数据集（如 Exodus、EnSight）中的材料/块选择。读者（Reader）用它暴露可选子集。 |

---

### 3.3 典型使用场景示例

1. 场景 1：在 `RequestInformation` 中设置全局范围
```cpp
int MyImageSource::RequestInformation(...) {
  int extent[6] = {0, 100, 0, 100, 0, 50};
  outputInfo->Set(vtkStreamingDemandDrivenPipeline::WHOLE_EXTENT(), extent, 6);
  outputInfo->Set(vtkDataObject::SPACING(), this->Spacing, 3);
  return 1;
}
```

2. 场景 2：在 `RequestData` 中创建点属性
```cpp
vtkNew<vtkDoubleArray> pressure;
pressure->SetName("Pressure");
pressure->SetNumberOfComponents(1);
pressure->SetNumberOfTuples(nPoints);

// 设置为活跃标量
vtkInformation* info = output->GetPointData()->GetAbstractArray("Pressure")->GetInformation();
info->Set(vtkDataObject::FIELD_ATTRIBUTE_TYPE(), vtkDataSetAttributes::SCALARS);
```

3. 场景 3：查询输入是否有法向量
```cpp
vtkPointData* pd = input->GetPointData();
if (pd->GetNormals()) {
  // 有法向量，可进行光照计算
}
// 或通过信息键查询
vtkInformationVector* pointVectors = inputInfo->Get(vtkDataObject::POINT_DATA_VECTOR());
```

---

## 4 认识vtkAlogorithm键对

在vtkAlgorithm中同样定义了一系列的key

```c++
/**
 * Keys used to specify input port requirements.
 * \ingroup InformationKeys
 */
static vtkInformationIntegerKey* INPUT_IS_OPTIONAL();
/**
 * \ingroup InformationKeys
 */
static vtkInformationIntegerKey* INPUT_IS_REPEATABLE();
/**
 * \ingroup InformationKeys
 */
static vtkInformationInformationVectorKey* INPUT_REQUIRED_FIELDS();
/**
 * \ingroup InformationKeys
 */
static vtkInformationStringVectorKey* INPUT_REQUIRED_DATA_TYPE();
/**
 * \ingroup InformationKeys
 */
static vtkInformationInformationVectorKey* INPUT_ARRAYS_TO_PROCESS();
/**
 * \ingroup InformationKeys
 */
static vtkInformationIntegerKey* INPUT_PORT();
/**
 * \ingroup InformationKeys
 */
static vtkInformationIntegerKey* INPUT_CONNECTION();

/**
 * This key tells the executive that a particular output port
 * is capable of producing an arbitrary subextent of the whole
 * extent. Many image sources and readers fall into this category
 * but some such as the legacy structured data readers cannot
 * support this feature.
 * \ingroup InformationKeys
 */
static vtkInformationIntegerKey* CAN_PRODUCE_SUB_EXTENT();

/**
 * Key that tells the pipeline that a particular algorithm
 * can or cannot handle piece request. If a filter cannot handle
 * piece requests and is asked for a piece, the executive will
 * flag an error. If a structured data source cannot handle piece
 * requests but can produce sub-extents (CAN_PRODUCE_SUB_EXTENT),
 * the executive will use an extent translator to split the extent
 * into pieces. Otherwise, if a source cannot handle piece requests,
 * the executive will ask for the whole data for piece 0 and not
 * execute the source for other pieces.
 * \ingroup InformationKeys
 */
static vtkInformationIntegerKey* CAN_HANDLE_PIECE_REQUEST();

```

---

### 4.1 输入端口约束类 Key

这些键用于在 `FillInputPortInformation()` 中声明 filter 对输入的要求。

1. `INPUT_IS_OPTIONAL()`
- **类型**：`vtkInformationIntegerKey`
- **作用**：标记某个输入端口是否**可选**（可以不连接）。
- **默认**：`0`（必需）；设为 `1` 表示可选。
- **示例**：
  ```cpp
  int MyFilter::FillInputPortInformation(int port, vtkInformation* info)
  {
    if (port == 1) // 第二个输入是可选的（如 mask）
    {
      info->Set(vtkAlgorithm::INPUT_IS_OPTIONAL(), 1);
    }
    return 1;
  }
  ```

> ✅ 适用于有主输入 + 辅助输入（如裁剪面、掩码）的 filter。

---

 2. `INPUT_IS_REPEATABLE()`
- **类型**：`vtkInformationIntegerKey`
- **作用**：标记该输入端口是否允许多个连接（即“多输入”）。
- **默认**：`0`（单输入）；设为 `1` 表示可重复连接。
- **效果**：用户可多次调用 `SetInputConnection(port, ...)`，所有连接会被收集到一个列表中。
- **典型应用**：`vtkAppendPolyData`、`vtkMultiBlockMergeFilter`。

---

3. `INPUT_REQUIRED_DATA_TYPE()`
- **类型**：`vtkInformationStringVectorKey`（注意：是 **VectorKey**！）
- **作用**：声明该输入端口**接受的数据类型列表**。
- **关键点**：
  - 是 **字符串向量**，可指定多个合法类型；
  - 管线连接时，VTK 会检查上游输出的 `DATA_TYPE_NAME` 是否在此列表中。
- **示例**：
  ```cpp
  info->Append(vtkAlgorithm::INPUT_REQUIRED_DATA_TYPE(), "vtkImageData");
  info->Append(vtkAlgorithm::INPUT_REQUIRED_DATA_TYPE(), "vtkRectilinearGrid");
  ```
  → 表示“接受图像或直角网格”。

> ⚠️ 注意：虽然名字叫 “DATA_TYPE”，但它属于 `vtkAlgorithm` 的输入约束，与输出的 `vtkDataObject::DATA_TYPE_NAME()` 是配对使用的。

---

4. `INPUT_REQUIRED_FIELDS()`
- **类型**：`vtkInformationInformationVectorKey`
- **作用**：声明输入**必须包含哪些字段**（如特定名称/类型的数组）。
- **结构**：每个元素是一个 `vtkInformation`，描述一个字段需求，包含：
  - `FIELD_NAME()`
  - `FIELD_ASSOCIATION()`
  - `FIELD_ATTRIBUTE_TYPE()` 等。
- **用途**：确保输入有 `"Normals"`、`"Velocity"` 等必要属性。
- **示例**（伪代码）：
  ```cpp
  vtkNew<vtkInformation> fieldReq;
  fieldReq->Set(vtkDataObject::FIELD_NAME(), "Temperature");
  fieldReq->Set(vtkDataObject::FIELD_ASSOCIATION(), vtkDataObject::FIELD_ASSOCIATION_POINTS);
  info->Append(vtkAlgorithm::INPUT_REQUIRED_FIELDS(), fieldReq);
  ```

> 🔍 实际使用较少，多数 filter 在 `RequestData` 中动态检查字段是否存在。

---

5. `INPUT_ARRAYS_TO_PROCESS()`
- **类型**：`vtkInformationInformationVectorKey`
- **作用**：由用户**指定要处理哪些数组**（而非 filter 强制要求）。
- **典型应用**：`vtkPassArrays`、`vtkAttributeCalculator` 等 filter。
- **机制**：
  - 用户通过 `AddInputArrayToProcess(...)` 设置；
  - filter 在 `RequestData` 中读取此列表，只处理指定数组。
- **不是约束，而是配置**。

---

6. `INPUT_PORT()` 与 `INPUT_CONNECTION()`
- **类型**：`vtkInformationIntegerKey`
- **作用**：在请求处理过程中，用于标识**当前处理的是哪个输入连接**。
- **内部使用**：通常由 `vtkExecutive` 自动设置，filter 很少直接读取。
- **场景**：多输入或多连接时，区分数据来源。

---

### 4.2 输出能力声明类 Key

这些键通常在 `RequestInformation` 阶段由 source/filter 设置，告诉下游它能提供什么能力。

1. `CAN_PRODUCE_SUB_EXTENT()`
- **类型**：`vtkInformationIntegerKey`
- **作用**：声明该算法**能否生成任意子范围**（sub-extent）的数据。
- **值**：`1` = 支持；`0` = 不支持（默认）。
- **典型支持者**：
  - `vtkImageReader2`（可读图像任意 ROI）；
  - `vtkRTAnalyticSource`（可生成任意区域）。
- **不支持者**：
  - 老式 reader（如 `vtkStructuredPointsReader`）必须读整个文件。
- **意义**：使 VTK 能实现 **流式处理、内存受限计算、并行分块**。

> ✅ 如果你的 source 可以按需生成局部数据，请设置此键为 `1`。

---

2. `CAN_HANDLE_PIECE_REQUEST()`
- **类型**：`vtkInformationIntegerKey`
- **作用**：声明该算法**能否直接响应“分块请求”**（piece request）。
- **背景**：
  - 在并行渲染中，每个进程可能只请求数据的某一块（piece）；
  - 如果 filter 不能处理 piece 请求，但支持 `CAN_PRODUCE_SUB_EXTENT`，VTK 会自动将 piece 转换为 extent 请求；
  - 如果两者都不支持，则只在 piece 0 上执行全量计算，其他 piece 为空。
- **值**：
  - `1`：我能直接处理 `DATA_PIECE_NUMBER`；
  - `0`：我不能（默认）。
- **典型设置**：
  ```cpp
  // 在 RequestInformation 中
  outputInfo->Set(CAN_HANDLE_PIECE_REQUEST(), 1);
  ```

> 💡 大多数自定义 filter 不需要处理 piece，除非你做并行/分布式开发。

---