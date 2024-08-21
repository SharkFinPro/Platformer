#include "Component.h"
#include "../Object.h"

Component::Component(const ComponentType type)
  : type{type}, owner{nullptr}
{}

ComponentType Component::getType() const
{
  return type;
}

void Component::setOwner(Object* owner)
{
  this->owner = owner;
}

Object* Component::getOwner() const
{
  return owner;
}

void Component::update([[maybe_unused]] const float dt)
{}

void Component::fixedUpdate([[maybe_unused]] const float dt)
{}
