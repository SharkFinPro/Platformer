#ifndef PLATFORMER_GAMEOBJECT_H
#define PLATFORMER_GAMEOBJECT_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "components/Component.h"

class GameObjectManager;

class GameObject
{
public:
    ~GameObject();

    void update(float dt);
    void fixedUpdate(float dt);

    void addComponent(Component* component);
    Component* getComponent(ComponentType type) const;

    void setOwner(GameObjectManager* objectOwner);
    GameObjectManager* getOwner() const;

protected:
    std::vector<Component*> components;
    GameObjectManager* owner;
};

#endif //PLATFORMER_GAMEOBJECT_H
