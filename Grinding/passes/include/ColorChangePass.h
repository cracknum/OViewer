#ifndef COLOR_CHANGE_PASS_H
#define COLOR_CHANGE_PASS_H
#include "PassesExport.h"
#include <memory>
#include <vtkOpenGLRenderPass.h>


class PASSES_API ColorChangePass : public vtkOpenGLRenderPass
{
public:
  static ColorChangePass* New();
  vtkTypeMacro(ColorChangePass, vtkOpenGLRenderPass);

  /**
   * Perform rendering according to a render state \p s.
   * \pre s_exists: s!=0
   */
  void Render(const vtkRenderState* s) override;

  /**
   * Release graphics resources and ask components to release their own
   * resources.
   * \pre w_exists: w!=0
   */
  void ReleaseGraphicsResources(vtkWindow* w) override;

protected:
  ColorChangePass();
  ~ColorChangePass() override;

private:
  struct Private;
  std::unique_ptr<Private> mPrivate;
};

#endif // COLOR_CHANGE_PASS_H