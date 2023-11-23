#ifndef PLATFORMER_RIGIDBODY_H
#define PLATFORMER_RIGIDBODY_H

#include "Component.h"
#include "../../math/Vec2.h"

class Transform;

class RigidBody : public Component
{
public:
  RigidBody();

  void fixedUpdate(double dt) override;

  void applyForce(Vec2<double> force);

  bool isFalling() const;

  void handleCollision(Vec2<double> penetrationVector);

private:
  Vec2<double> velocity;

  bool doGravity;
  double gravity;

  bool falling;

  Transform* transform;

  void limitMovement();

  void handleXCollision();
  void handleYCollision();
};


#endif //PLATFORMER_RIGIDBODY_H
