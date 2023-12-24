#ifndef PLATFORMER_OBJECTMANAGER_H
#define PLATFORMER_OBJECTMANAGER_H

#include <SFML/Graphics.hpp>
#include <vector>
class Object;

class ObjectManager
{
public:
  ObjectManager();
  ~ObjectManager();

  void update(float dt);

  void addObject(Object* object);
  void removeObject(Object* object);

  void setWindow(sf::RenderWindow* window);
  [[nodiscard]] sf::RenderWindow* getWindow() const;

private:
  std::vector<Object*> objects;
  sf::RenderWindow* window;

  const float fixedUpdateDt;
  float timeAccumulator;
  int ticks;

  void variableUpdate(float dt);
  void fixedUpdate(float dt);

  void checkCollisions();
};

#endif //PLATFORMER_OBJECTMANAGER_H
