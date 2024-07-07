#ifndef PLATFORMER_OBJECTMANAGER_H
#define PLATFORMER_OBJECTMANAGER_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>

class Object;
class Collider;
class RigidBody;

class ObjectManager
{
public:
  ObjectManager();

  void update(float dt);

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

  void variableUpdate(float dt);
  void fixedUpdate(float dt);

  void checkCollisions();

  void findCollisions(const std::shared_ptr<Object>& object1, const std::shared_ptr<Collider>& collider, std::vector<std::shared_ptr<Object>>& collidedObjects);

  void handleCollisions(const std::shared_ptr<RigidBody>& rigidBody, const std::shared_ptr<Collider>& collider, std::vector<std::shared_ptr<Object>>& collidedObjects);
};

#endif //PLATFORMER_OBJECTMANAGER_H
