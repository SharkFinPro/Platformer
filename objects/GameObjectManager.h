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

  void update(double dt);

  void addObject(GameObject* object);
  void removeObject(GameObject* object);

  void setWindow(sf::RenderWindow* window);
  sf::RenderWindow* getWindow() const;

private:
  std::vector<GameObject*> objects;
  sf::RenderWindow* window;

  const double fixedUpdateDt;
  double timeAccumulator;
  int ticks;

  void variableUpdate(double dt);
  void fixedUpdate(double dt);

  void checkCollisions();
};

#endif //PLATFORMER_GAMEOBJECTMANAGER_H
