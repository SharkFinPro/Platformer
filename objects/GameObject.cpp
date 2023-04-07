#include "GameObject.h"
#include "components/Component.h"
#include "components/SpriteRenderer.h"
#include <cstring>

GameObject::~GameObject()
{
    for (auto component : components)
        delete component;
}

void GameObject::draw(sf::RenderWindow* window) const
{
    auto renderer = dynamic_cast<SpriteRenderer*>(getComponent(ComponentType::spriteRenderer));

    if (renderer)
        renderer->draw(window);
}

void GameObject::update(float dt)
{
    for (auto component : components)
        component->update(dt);
}

void GameObject::fixedUpdate(float dt)
{
    for (auto component : components)
        component->fixedUpdate(dt);
}

void GameObject::addComponent(Component* component)
{
    component->setOwner(this);
    components.push_back(component);
}

Component* GameObject::getComponent(const ComponentType type) const
{
    for (auto component : components)
        if (component->getType() == type)
            return component;

    return nullptr;
}
