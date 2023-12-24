#ifndef PLATFORMER_RIGIDBODY_H
#define PLATFORMER_RIGIDBODY_H

#include "Component.h"
#include "../../math/Vec2.h"

class Transform;

class RigidBody : public Component
{
public:
  RigidBody();

  void fixedUpdate(float dt) override;

  void applyForce(const Vec2<float>& force);

  [[nodiscard]] bool isFalling() const;

  void handleCollision(Vec2<float> minimumTranslationVector);

private:
  Vec2<float> velocity;

  bool doGravity;
  Vec2<float> gravity;

  bool falling;

  Transform* transform;

  void limitMovement();

  void handleXCollision();
  void handleYCollision(float minimumTranslationVector);
};


#endif //PLATFORMER_RIGIDBODY_H
