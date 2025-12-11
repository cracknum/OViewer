#include "PolyDataMapper.h"
#include "PlaneSource.h"
#include "Quad.h"
struct PolyDataMapper::Private
{
  std::unique_ptr<Quad> mQuadPrimitive;

  explicit Private(const std::shared_ptr<ShaderProgramManager>& shaderManager)
  {
    mQuadPrimitive = std::make_unique<Quad>(shaderManager);
  }
};

PolyDataMapper::PolyDataMapper(const std::shared_ptr<ShaderProgramManager>& shaderManager)
{
  mPrivate = std::make_unique<Private>(shaderManager);
}

PolyDataMapper::~PolyDataMapper() = default;

void PolyDataMapper::setDataSource(const std::shared_ptr<PlaneSource>& planeSource)
{
  if (!planeSource)
  {
    return;
  }

  mPrivate->mQuadPrimitive->setVertices(planeSource->polyData());
  mPrivate->mQuadPrimitive->setModelMatrix(planeSource->indexToWorldMatrix());
}

void PolyDataMapper::render()
{
  mPrivate->mQuadPrimitive->setViewMatrix(mViewMatrix);
  mPrivate->mQuadPrimitive->setProjectMatrix(mProjectMatrix);
  mPrivate->mQuadPrimitive->draw();
}