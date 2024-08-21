#ifndef PLATFORMER_COMPONENT_H
#define PLATFORMER_COMPONENT_H

class Object;

enum class ComponentType {
  collider,
  player,
  rigidBody,
  spriteRenderer,
  transform
};

class Component
{
public:
  explicit Component(ComponentType type);
  virtual ~Component() = default;

  [[nodiscard]] ComponentType getType() const;

  void setOwner(Object* owner);
  [[nodiscard]] Object* getOwner() const;

  virtual void update(float dt);
  virtual void fixedUpdate(float dt);

protected:
  ComponentType type;
  Object* owner;
};

#endif //PLATFORMER_COMPONENT_H
