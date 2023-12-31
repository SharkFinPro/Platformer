#include "RigidBody.h"
#include "../Object.h"
#include "Transform.h"

RigidBody::RigidBody()
  : Component{ComponentType::rigidBody}, velocity{0}, doGravity{true}, gravity{0.0f, 9.81f, 0.0f}, falling{true}
{}

void RigidBody::fixedUpdate(float dt)
{
  if (transform_ptr.expired())
  {
    transform_ptr = dynamic_pointer_cast<Transform>(owner->getComponent(ComponentType::transform));

    if (transform_ptr.expired())
      return;
  }

  if (std::shared_ptr<Transform> transform = transform_ptr.lock())
  {
    falling = true;

    if (doGravity)
      applyForce(gravity);

    limitMovement();

    transform->move(velocity * dt);
  }
}

void RigidBody::applyForce(const Vec3<float>& force)
{
  velocity += force;
}

void RigidBody::limitMovement()
{
  applyForce({-velocity.getX() * 0.05f, 0, 0});
}

bool RigidBody::isFalling() const
{
  return falling;
}

void RigidBody::handleCollision(Vec3<float> minimumTranslationVector)
{
  if (transform_ptr.expired())
  {
    transform_ptr = dynamic_pointer_cast<Transform>(owner->getComponent(ComponentType::transform));

    if (transform_ptr.expired())
      return;
  }

  if (std::shared_ptr<Transform> transform = transform_ptr.lock())
  {
    if (minimumTranslationVector.getX() != 0)
      handleXCollision();

    if (minimumTranslationVector.getY() != 0)
      handleYCollision(minimumTranslationVector.getY());

    transform->move(minimumTranslationVector);
  }
}

void RigidBody::handleXCollision()
{
  velocity.setX(0);
}

void RigidBody::handleYCollision(float minimumTranslationVector)
{
  if (minimumTranslationVector < 0)
    falling = false;

  velocity.setY(0);
}
