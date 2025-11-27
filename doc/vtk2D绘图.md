# vtk 2D 绘图

## vtkPolyDataFS, vtkPolyDataVS, vtkPolyDataEdgesGS shader source对应的placement
## 涉及到的类
vtkOpenGLPolyDataMapper2D
    - 支持VS, FS, GS
    - 当使用了actor->GetProperty()->GetLineWidth() > 1.0并且绘制为Lines时才会启用GS，也就是说vtkOpenGLPolyDataMapper2D是支持设置线宽的
vtkActor2D
vtkTextureObject
vtkOpenGLHelper

```c++
class VTKRENDERINGOPENGL2_EXPORT vtkOpenGLHelper
{
public:
  vtkShaderProgram* Program;
  vtkTimeStamp ShaderSourceTime;
  vtkOpenGLVertexArrayObject* VAO;
  vtkTimeStamp AttributeUpdateTime;
  int PrimitiveType;
  unsigned int ShaderChangeValue;

  vtkOpenGLIndexBufferObject* IBO;

  vtkOpenGLHelper();
  ~vtkOpenGLHelper();
  void ReleaseGraphicsResources(vtkWindow* win);

private:
  vtkOpenGLHelper(const vtkOpenGLHelper&) = delete;
  vtkOpenGLHelper& operator=(const vtkOpenGLHelper&) = delete;
};
```
