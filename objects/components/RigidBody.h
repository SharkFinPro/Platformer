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

  void applyForce(float x, float y);

  bool isFalling() const;

  void handleCollision(Vec2<float> penetrationVector);

private:
  Vec2<float> velocity;

  bool doGravity;
  float gravity;

  bool falling;

  Transform* transform;

  void limitMovement();

  void handleXCollision();
  void handleYCollision();
};


#endif //PLATFORMER_RIGIDBODY_H
