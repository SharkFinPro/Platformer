#ifndef PLATFORMER_OBJECTMANAGER_H
#define PLATFORMER_OBJECTMANAGER_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>

class Object;

class ObjectManager
{
public:
  ObjectManager();

  void update(const float& dt);

  void addObject(std::shared_ptr<Object> object);

  [[maybe_unused]] bool removeObject(const std::shared_ptr<Object>& object);

  void setWindow(sf::RenderWindow* window);
  [[nodiscard]] sf::RenderWindow* getWindow() const;

private:
  std::vector<std::shared_ptr<Object>> objects;
  sf::RenderWindow* window;

  const float fixedUpdateDt;
  float timeAccumulator;
  int ticks;

  void variableUpdate(const float& dt);
  void fixedUpdate(const float& dt);

  void checkCollisions();
};

#endif //PLATFORMER_OBJECTMANAGER_H
