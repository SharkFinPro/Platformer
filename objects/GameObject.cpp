#include "GameObject.h"
#include "components/Component.h"

GameObject::~GameObject()
{
    for (auto [componentType, component] : components)
    {
        delete component;
        component = nullptr;
    }
}

void GameObject::update(float dt)
{
    for (auto [componentType, component] : components)
        component->update(dt);
}

void GameObject::fixedUpdate(float dt)
{
    for (auto [componentType, component] : components)
        component->fixedUpdate(dt);
}

void GameObject::addComponent(Component* component)
{
    component->setOwner(this);
    components.insert({ component->getType(), component });
}

Component* GameObject::getComponent(const ComponentType type) const
{
    for (auto [componentType, component] : components)
        if (componentType == type)
            return component;

    return nullptr;
}

void GameObject::setOwner(GameObjectManager* objectOwner)
{
    owner = objectOwner;
}

GameObjectManager* GameObject::getOwner() const
{
    return owner;
}
