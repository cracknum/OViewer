#ifndef PLANE_TEXTURE_H
#define PLANE_TEXTURE_H
#include <vtkOpenGLTexture.h>
class PlaneTexture : public vtkOpenGLTexture
{
public:
  static PlaneTexture* New();
  vtkTypeMacro(PlaneTexture, vtkOpenGLTexture);
  void PrintSelf(ostream& os, vtkIndent indent) override;

  /**
   * Implement base class method.
   */
  void Load(vtkRenderer*) override;

  /**
   * Implement base class method.
   */
  void Render(vtkRenderer* ren) override { this->Load(ren); }

  void ReleaseGraphicsResources(vtkWindow*) override;

protected:
  PlaneTexture() = default;
  ~PlaneTexture() override;

private:
  PlaneTexture(const PlaneTexture&) = delete;
  void operator=(const PlaneTexture&) = delete;
};
#endif