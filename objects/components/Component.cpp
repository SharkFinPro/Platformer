#include "Component.h"
#include "../Object.h"

Component::Component(ComponentType type_)
  : type{type_}, owner{nullptr}
{}

ComponentType Component::getType() const
{
  return type;
}

void Component::setOwner(Object* owner_)
{
  owner = owner_;
}

Object* Component::getOwner() const
{
  return owner;
}

void Component::update([[maybe_unused]] const float& dt)
{}

void Component::fixedUpdate([[maybe_unused]] const float& dt)
{}
