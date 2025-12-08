#include "PolyDataMapper.h"
#include "PlaneSource.h"
#include "Quad.h"
struct PolyDataMapper::Private
{
  std::unique_ptr<Quad> mQuadPrimitive;

  Private() { mQuadPrimitive = std::make_unique<Quad>(); }
};

PolyDataMapper::PolyDataMapper()
{
  mPrivate = std::make_unique<Private>();
}

PolyDataMapper::~PolyDataMapper() {}

void PolyDataMapper::setDataSource(const std::shared_ptr<PlaneSource> planeSource) {
	if (!planeSource)
	{
		return;
	}

	mPrivate->mQuadPrimitive->setVertices(planeSource->polyData());
	mPrivate->mQuadPrimitive->setModelMatrix(planeSource->indexToWorldMatrix());
}

void PolyDataMapper::render() {}