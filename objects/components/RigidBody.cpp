#include "RigidBody.h"
#include "../Object.h"
#include "Transform.h"

RigidBody::RigidBody()
  : Component{ComponentType::rigidBody}, velocity{0}, doGravity{true}, gravity{0, 9.81f, 0}, falling{true},
    wasFalling{true}, wasWasFalling(true)
{}

void RigidBody::fixedUpdate(const float dt)
{
  if (transform_ptr.expired())
  {
    transform_ptr = dynamic_pointer_cast<Transform>(owner->getComponent(ComponentType::transform));

    if (transform_ptr.expired())
    {
      return;
    }
  }

  if (const std::shared_ptr<Transform> transform = transform_ptr.lock())
  {
    wasWasFalling = wasFalling;

    wasFalling = true;

    falling = true;

    if (doGravity)
    {
      applyForce(gravity * dt);
    }

    limitMovement();

    transform->move(velocity);
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

void RigidBody::handleCollision(const Vec3<float> minimumTranslationVector, const std::shared_ptr<Object>& other)
{
  if (transform_ptr.expired())
  {
    transform_ptr = dynamic_pointer_cast<Transform>(owner->getComponent(ComponentType::transform));

    if (transform_ptr.expired())
    {
      return;
    }
  }

  if (const std::shared_ptr<Transform> transform = transform_ptr.lock())
  {
    if (minimumTranslationVector.getY() < 0 && std::fabs(minimumTranslationVector.getY()) > 0.001f)
    {
      if (!wasWasFalling)
      {
        falling = false;
      }
      else
      {
        wasFalling = false;
      }
    }

    if (other != nullptr)
    {
      if (const auto otherRb = dynamic_pointer_cast<RigidBody>(other->getComponent(ComponentType::rigidBody)))
      {
        otherRb->handleCollision(-minimumTranslationVector, nullptr);
      }
    }

    velocity += minimumTranslationVector;

    transform->move(minimumTranslationVector);
  }
}
