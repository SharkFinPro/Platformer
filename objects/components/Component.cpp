#include "Component.h"
#include "../Object.h"

Component::Component(ComponentType type)
  : type{type}, owner{nullptr}
{}

ComponentType Component::getType() const
{
  return type;
}

void Component::setOwner(Object* componentOwner)
{
  owner = componentOwner;
}

Object* Component::getOwner() const
{
  return owner;
}

void Component::update([[maybe_unused]] float dt)
{}

void Component::fixedUpdate([[maybe_unused]] float dt)
{}
