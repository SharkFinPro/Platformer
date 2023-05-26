#ifndef PLATFORMER_GAMEOBJECTMANAGER_H
#define PLATFORMER_GAMEOBJECTMANAGER_H

#include <SFML/Graphics.hpp>
#include <vector>
class GameObject;

class GameObjectManager
{
public:
    GameObjectManager();
    ~GameObjectManager();

    void update(float dt);

    void addObject(GameObject* object);
    void removeObject(GameObject* object);

    void setWindow(sf::RenderWindow* window);
    sf::RenderWindow* getWindow() const;

private:
    std::vector<GameObject*> objects;
    sf::RenderWindow* window;

    const float fixedUpdateDt;
    float timeAccumulator;
    int ticks;

    void variableUpdate(float dt);
    void fixedUpdate(float dt);

    void checkCollisions(float dt);
};

#endif //PLATFORMER_GAMEOBJECTMANAGER_H
