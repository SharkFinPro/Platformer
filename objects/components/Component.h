#ifndef PLATFORMER_COMPONENT_H
#define PLATFORMER_COMPONENT_H

class Object;

enum class ComponentType { boxCollider, player, rigidBody, spriteRenderer, transform, collider };

class Component
{
public:
  explicit Component(ComponentType type);
  virtual ~Component() = default;

  ComponentType getType() const;

  void setOwner(Object* componentOwner);
  Object* getOwner() const;

  virtual void update(float dt);
  virtual void fixedUpdate(float dt);

protected:
  ComponentType type;
  Object* owner;
};

#endif //PLATFORMER_COMPONENT_H
