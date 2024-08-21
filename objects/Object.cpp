#include "Object.h"
#include "components/Component.h"

Object::Object()
  : owner(nullptr)
{}

void Object::update(const float dt)
{
  for (auto& [componentType, component] : components)
  {
    component->update(dt);
  }
}

void Object::fixedUpdate(const float dt)
{
  for (auto& [componentType, component] : components)
  {
    component->fixedUpdate(dt);
  }
}

void Object::addComponent(std::shared_ptr<Component> component)
{
  component->setOwner(this);
  components.insert({ component->getType(), std::move(component) });
}

std::shared_ptr<Component> Object::getComponent(const ComponentType type) const
{
  if (const auto component = components.find(type); component != components.end())
  {
    return component->second;
  }

  return nullptr;
}

void Object::setOwner(ObjectManager* objectOwner)
{
  owner = objectOwner;
}

ObjectManager* Object::getOwner() const
{
  return owner;
}
