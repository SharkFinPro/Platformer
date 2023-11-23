#ifndef PLATFORMER_GAMEOBJECT_H
#define PLATFORMER_GAMEOBJECT_H

#include <SFML/Graphics.hpp>
#include <string>
#include <unordered_map>

class GameObjectManager;
enum class ComponentType;
class Component;

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
  std::unordered_map<ComponentType, Component*> components;
  GameObjectManager* owner;
};

#endif //PLATFORMER_GAMEOBJECT_H
