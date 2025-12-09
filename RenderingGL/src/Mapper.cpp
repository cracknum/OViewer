#include "Mapper.h"

struct Mapper::Private
{
};

Mapper::Mapper()
{
  mPrivate = std::make_unique<Private>();
}

Mapper::~Mapper() {}
