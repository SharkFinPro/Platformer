#ifndef PLATFORMER_RIGIDBODY_H
#define PLATFORMER_RIGIDBODY_H

#include "Component.h"
#include "../../math/Vec3.h"
#include <memory>

class Transform;

class RigidBody : public Component
{
public:
  RigidBody();

  void fixedUpdate(float dt) override;

  void applyForce(const Vec3<float>& force);

  [[nodiscard]] bool isFalling() const;

  void handleCollision(Vec3<float> minimumTranslationVector, const std::shared_ptr<Object>& other);

private:
  Vec3<float> velocity;

  bool doGravity;
  Vec3<float> gravity;

  bool falling;
  bool wasFalling;
  bool wasWasFalling;

  std::weak_ptr<Transform> transform_ptr;

  void limitMovement();
};


#endif //PLATFORMER_RIGIDBODY_H
