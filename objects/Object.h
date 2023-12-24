#ifndef PLATFORMER_OBJECT_H
#define PLATFORMER_OBJECT_H

#include <SFML/Graphics.hpp>
#include <string>
#include <unordered_map>

class ObjectManager;
enum class ComponentType;
class Component;

class Object
{
public:
  ~Object();

  void update(float dt);
  void fixedUpdate(float dt);

  void addComponent(Component* component);
  Component* getComponent(ComponentType type) const;

  void setOwner(ObjectManager* objectOwner);
  ObjectManager* getOwner() const;

protected:
  std::unordered_map<ComponentType, Component*> components;
  ObjectManager* owner;
};

#endif //PLATFORMER_OBJECT_H
