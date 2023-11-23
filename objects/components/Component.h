#ifndef PLATFORMER_COMPONENT_H
#define PLATFORMER_COMPONENT_H

class GameObject;

enum class ComponentType { boxCollider, player, rigidBody, spriteRenderer, transform };

class Component
{
public:
  explicit Component(ComponentType type);
  virtual ~Component() = default;

  ComponentType getType() const;

  void setOwner(GameObject* componentOwner);
  GameObject* getOwner() const;

  virtual void update(double dt);
  virtual void fixedUpdate(double dt);

protected:
  ComponentType type;
  GameObject* owner;
};

#endif //PLATFORMER_COMPONENT_H
