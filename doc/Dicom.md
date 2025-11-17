# Dicom相关总结

## 1. study和series的关系
Patient（患者）
└── Study（检查）
    └── Series（序列）
        └── Instance（实例，即单张图像或一个图像帧）

## 2. Slice thickness和spacing between slices的区别
在 DICOM 标准中，**(0018,0050) Slice Thickness** 和 **(0018,0088) Spacing Between Slices** 都用于描述医学影像数据的空间属性，但它们代表的具体含义和用途是不同的。以下是这两个标签的详细区别：

### 1. **(0018,0050) Slice Thickness（层厚）**

- **定义**：指定了单个图像切片（Slice）的物理厚度，即扫描过程中实际采集的体素（Voxel）在Z轴方向上的尺寸。
- **单位**：毫米（mm）
- **作用**：
  - 表示了成像设备在获取图像时，沿患者身体长轴方向（通常是Z轴）所覆盖的实际物理距离。
  - 它影响到重建图像的分辨率以及能够显示的细节程度。例如，在CT或MRI扫描中，较薄的层厚可以提供更高的空间分辨率，但可能会增加噪声；而较厚的层厚则相反。
- **例子**：如果 Slice Thickness 是 `5 mm`，这意味着每个切片在Z轴上覆盖了5毫米的距离。

### 2. **(0018,0088) Spacing Between Slices（层间距）**

- **定义**：表示相邻两个切片中心点之间的物理距离。
- **单位**：毫米（mm）
- **作用**：
  - 描述了连续切片间的间隔，这有助于确定整个扫描区域内的切片分布情况。
  - 在某些情况下，Spacing Between Slices 可能等于 Slice Thickness（等距采样），但在其他情况下，尤其是当使用非连续扫描技术时，这两个值可能不同。
  - 如果 Spacing Between Slices 大于 Slice Thickness，则意味着存在未被扫描覆盖的间隙；反之，若小于 Slice Thickness，则可能存在重叠。
- **例子**：**如果 Spacing Between Slices 是 `7 mm`，而 Slice Thickness 是 `5 mm`，那么每张切片之间有2毫米的空隙没有被扫描覆盖。**

### 📊 区别总结

| 属性 | (0018,0050) Slice Thickness | (0018,0088) Spacing Between Slices |
|------|-----------------------------|------------------------------------|
| **含义** | 单个切片的实际物理厚度 | 相邻切片中心点之间的距离 |
| **关系** | 影响单张切片内部的信息密度 | 决定切片之间的相对位置 |
| **应用场景** | 选择合适的扫描参数以平衡分辨率与噪声 | 确定是否需要进行插值处理来填补空白区域 |

## 3. Image Type
DICOM 标签 **(0008,0008) Image Type** 是一个非常重要的元数据字段，用于描述图像的**来源、处理状态和用途**。它是一个**多值字符串（Multi-valued String）**，通常包含 **2 到 6 个用反斜杠 `\` 分隔的组件（components）**，每个组件表示图像在不同维度上的分类。

---

### ✅ 基本格式

```text
Image Type = [Component1 \ Component2 \ Component3 \ ...]
```

最常见的形式是 **两个或三个组件**，例如：

- `ORIGINAL\PRIMARY`
- `DERIVED\SECONDARY`
- `ORIGINAL\PRIMARY\AXIAL`
- `DERIVED\PRIMARY\REFORMATTED\AVERAGE`

---

### 🔍 各组件含义（按位置）

#### **第1个组件：像素数据的来源**
表示图像数据是否为原始采集还是经过处理生成。

| 值 | 含义 |
|----|------|
| `ORIGINAL` | 图像数据直接来自扫描设备的原始采集（未经重建或后处理） |
| `DERIVED`  | 图像由其他图像通过算法生成（如MPR、MIP、平均、分割、AI重建等） |
| `SECONDARY CAPTURE` | 非DICOM设备捕获的图像（如屏幕截图、照片），已转换为DICOM格式 |

> 💡 注意：`SECONDARY CAPTURE` 通常作为整个 Image Type 的唯一值（即只有一个组件），用于非影像设备生成的图像。

---

#### **第2个组件：图像在检查中的角色**
表示该图像与患者检查计划的关系。

| 值 | 含义 |
|----|------|
| `PRIMARY`   | 属于本次检查计划中预期采集的标准图像（常规序列） |
| `SECONDARY` | 非计划内生成的图像（如技师额外重建、PACS后处理生成） |
| `LOCALIZER` | 定位像（scout view），用于规划后续扫描范围 |

---

#### **第3个及以后组件：可选的附加信息**
这些是厂商或模态特定的扩展，用于进一步描述图像特性，例如：

| 示例值 | 含义 |
|--------|------|
| `AXIAL`, `SAGITTAL`, `CORONAL` | 重建方向（常用于 DERIVED 图像） |
| `REFORMATTED` | 表示是重采样/重格式化图像 |
| `VOLUME`, `PROJECTION IMAGE` | 图像类型（如CT容积、DR投影） |
| `EMISSION`, `TRANSMISSION` | 核医学中的成像方式 |
| `AVERAGE`, `MAXIMUM`, `MINIP` | 后处理方法（如平均、MIP、MinIP） |

---

### 📌 常见组合示例

| Image Type | 解释 |
|-----------|------|
| `ORIGINAL\PRIMARY` | 最常见的原始图像，如CT轴扫、MR原始序列 |
| `DERIVED\PRIMARY` | 虽为衍生图，但属于检查协议的一部分（如CT自动重建的冠状面） |
| `DERIVED\SECONDARY` | 检查后人工或系统额外生成的图像（如放射科医生重建的MPR） |
| `ORIGINAL\PRIMARY\AXIAL` | 原始轴位图像（部分厂商会加上方向） |
| `DERIVED\PRIMARY\REFORMATTED\CORONAL` | 系统自动生成的冠状位重建图 |
| `SECONDARY CAPTURE` | 手术照片、超声截图等转成的DICOM文件 |

---

### ⚠️ 注意事项

1. **顺序固定**：组件顺序有严格定义，不能随意调换。
2. **不是所有图像都有多个组件**：有些老设备或简单模态只写前两个。
3. **对临床和科研很重要**：
   - AI模型训练时通常只用 `ORIGINAL\PRIMARY`，避免引入处理偏差。
   - PACS归档策略可能区别对待 `DERIVED` 图像。
   - 定量分析（如CT值测量）应避免使用 `DERIVED` 图像（可能失真）。

## 4. samples per pixel
是的，**DICOM 标签 (0028,0002) Samples per Pixel（每像素采样数）通常与图像的“通道数”（number of channels）是一致的**，但需要结合其他字段（尤其是 **Photometric Interpretation**）来正确理解其含义。

---

### ✅ 基本对应关系

| Samples per Pixel | 通常对应的图像类型 | 通道解释 |
|-------------------|------------------|--------|
| **1** | 灰度图像（Grayscale） | 单通道（如 CT、MR、X光） |
| **3** | 彩色图像（Color） | 三通道（通常是 RGB 或 YBR） |

> 📌 所以，在绝大多数实际应用中：  
> **Samples per Pixel = 图像通道数**

---

### 🔍 但要注意：必须结合 **(0028,0004) Photometric Interpretation**

因为 **Samples per Pixel = 3** 并不总是意味着“RGB”，还可能是其他色彩空间。关键要看 **Photometric Interpretation** 字段：

#### 常见组合示例：

| Samples per Pixel | Photometric Interpretation | 含义 |
|-------------------|----------------------------|------|
| 1 | `MONOCHROME1` 或 `MONOCHROME2` | 灰度图（单通道）<br>`MONOCHROME2`：值越大越亮（常规）<br>`MONOCHROME1`：值越大越暗 |
| 3 | `RGB` | 真彩色图像，三个样本 = R, G, B |
| 3 | `YBR_FULL` / `YBR_FULL_422` / `YBR_PARTIAL_422` | YCbCr 色彩空间（常用于 JPEG 压缩的 DICOM），需转换为 RGB 才能正确显示 |

> 💡 例如：超声（US）或皮肤镜图像常以 `Samples per Pixel = 3` + `Photometric Interpretation = RGB` 存储。

---

### ⚠️ 特殊情况说明

1. **Palette Color（调色板图像）**：
   - `Samples per Pixel = 1`
   - 但通过 **(0028,1101–1103) Palette Color Lookup Table** 映射为彩色
   - 此时虽然物理存储是单通道，但视觉上是彩色 → **逻辑通道 ≠ 存储通道**
   - 这类图像较少见，多见于老式设备或核医学

2. **多光谱/高光谱图像（极罕见）**：
   - DICOM 支持 `Samples per Pixel > 3`（如 4、8、甚至上百）
   - 用于科研或特殊成像（如荧光成像、病理多光谱）
   - 普通临床影像几乎不会遇到