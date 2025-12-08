#ifndef POLY_DATA_MAPPER_H
#define POLY_DATA_MAPPER_H
#include <Mapper.h>
#include <memory>
#include "RenderingGLExport.h"

class PlaneSource;

class RENDERINGGL_API PolyDataMapper : public Mapper
{
public:
  PolyDataMapper();
  ~PolyDataMapper();
  void setDataSource(const std::shared_ptr<PlaneSource> planeSource);
  void render() override;

private:
  struct Private;
  std::unique_ptr<Private> mPrivate;
};
#endif // POLY_DATA_MAPPER_H