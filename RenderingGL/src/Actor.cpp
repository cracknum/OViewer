#include "Actor.h"
#include "Mapper.h"
struct Actor::Private
{
  std::shared_ptr<Mapper> mMapper;
};

Actor::Actor()
{
  mPrivate = std::make_unique<Private>();
}

Actor::~Actor() {}

void Actor::setMapper(const std::shared_ptr<Mapper>& mapper)
{
  mPrivate->mMapper = mapper;
}
std::shared_ptr<Mapper> Actor::mapper()
{
  return mPrivate->mMapper;
}
