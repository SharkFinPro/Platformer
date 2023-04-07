#ifndef PLATFORMER_GAMEOBJECT_H
#define PLATFORMER_GAMEOBJECT_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "components/Component.h"

class GameObject
{
public:
    ~GameObject();

    void draw(sf::RenderWindow* window) const;
    void update(float dt);
    void fixedUpdate(float dt);

    void addComponent(Component* component);
    Component* getComponent(ComponentType type) const;

protected:
    std::vector<Component*> components;
};

#endif //PLATFORMER_GAMEOBJECT_H
