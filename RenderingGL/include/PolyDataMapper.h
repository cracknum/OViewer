#ifndef POLY_DATA_MAPPER_H
#define POLY_DATA_MAPPER_H
#include "RenderingGLExport.h"
#include <Mapper.h>
#include <memory>

class PlaneSource;
class ShaderManager;

class RENDERINGGL_API PolyDataMapper final : public Mapper
{
public:
  explicit PolyDataMapper(const std::shared_ptr<ShaderManager>& shaderManager);
  ~PolyDataMapper() override;
  void setDataSource(const std::shared_ptr<PlaneSource>& planeSource);
  void render() override;

private:
  struct Private;
  std::unique_ptr<Private> mPrivate;
};
#endif // POLY_DATA_MAPPER_H