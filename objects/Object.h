#ifndef PLATFORMER_OBJECT_H
#define PLATFORMER_OBJECT_H

#include <unordered_map>
#include <memory>
#include <vector>

class ObjectManager;
enum class ComponentType;
class Component;

class Object
{
public:
  Object();
  explicit Object(const std::vector<std::shared_ptr<Component>>& components);

  void update(float dt);
  void fixedUpdate(float dt);

  void addComponent(std::shared_ptr<Component> component);
  std::shared_ptr<Component> getComponent(ComponentType type) const;

  void setOwner(ObjectManager* objectOwner);
  ObjectManager* getOwner() const;

protected:
  std::unordered_map<ComponentType, std::shared_ptr<Component>> components;
  ObjectManager* owner;
};

#endif //PLATFORMER_OBJECT_H
