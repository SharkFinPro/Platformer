#ifndef PLATFORMER_GAMEOBJECT_H
#define PLATFORMER_GAMEOBJECT_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
class Component;

class GameObject
{
public:
    ~GameObject();

    void draw(sf::RenderWindow* window) const;
    void update(float dt);
    void fixedUpdate(float dt);

    void addComponent(Component* component);
    Component* getComponent(const std::string& name) const;

protected:
    std::vector<Component*> components;
};

#endif //PLATFORMER_GAMEOBJECT_H
