#include "Component.h"
#include "../GameObject.h"

Component::Component(ComponentType type)
  : type{type}, owner{nullptr}
{}

ComponentType Component::getType() const
{
  return type;
}

void Component::setOwner(GameObject* componentOwner)
{
  owner = componentOwner;
}

GameObject* Component::getOwner() const
{
  return owner;
}

void Component::update(float dt)
{}

void Component::fixedUpdate(float dt)
{}
