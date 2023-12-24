#include "Object.h"
#include "components/Component.h"

Object::~Object()
{
  for (auto [componentType, component] : components)
  {
    delete component;
    component = nullptr;
  }
}

void Object::update(float dt)
{
  for (auto [componentType, component] : components)
    component->update(dt);
}

void Object::fixedUpdate(float dt)
{
  for (auto [componentType, component] : components)
    component->fixedUpdate(dt);
}

void Object::addComponent(Component* component)
{
  component->setOwner(this);
  components.insert({ component->getType(), component });
}

Component* Object::getComponent(const ComponentType type) const
{
  for (auto [componentType, component] : components)
    if (componentType == type)
      return component;

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
