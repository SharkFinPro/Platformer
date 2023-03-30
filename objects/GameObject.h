#ifndef PLATFORMER_GAMEOBJECT_H
#define PLATFORMER_GAMEOBJECT_H

#include <SFML/Graphics.hpp>
#include <vector>
class Component;

class GameObject
{
public:
    ~GameObject();

    void draw(sf::RenderWindow* window) const;
    void update(float dt);
    void fixedUpdate(float dt);

    void addComponent(Component* component);
    Component* getComponent(const char* name) const;

protected:
    std::vector<Component*> components;
};

#endif //PLATFORMER_GAMEOBJECT_H
