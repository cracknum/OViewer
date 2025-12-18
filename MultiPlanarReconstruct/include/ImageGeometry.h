#ifndef IMAGE_GEOMETRY_H
#define IMAGE_GEOMETRY_H
#include "BaseGeometry.h"
#include <memory>

class vtkImageData;
class ImageGeometry : public BaseGeometry
{
public:
  static ImageGeometry* New();
  vtkTypeMacro(ImageGeometry, BaseGeometry);
  
  void initializeImage(vtkImageData* imageData);
protected:
  ImageGeometry();
  ~ImageGeometry() override;

private:
  struct Private;
  std::unique_ptr<Private> mPrivate;
}

#endif // IMAGE_GEOMETRY_H