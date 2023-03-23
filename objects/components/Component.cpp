#include "Component.h"
#include "../GameObject.h"

Component::Component(const char* name)
    : name{name}, owner{nullptr}
{}

const char* Component::getName() const
{
    return name;
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
