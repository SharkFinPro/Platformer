#include "RigidBody.h"
#include "../Object.h"
#include "Transform.h"

RigidBody::RigidBody()
  : Component{ComponentType::rigidBody}, velocity{0, 0}, doGravity{true}, gravity{0.0f, 9.81f}, falling{true}, transform{nullptr}
{}

void RigidBody::fixedUpdate(float dt)
{
  if (!transform)
  {
    transform = dynamic_cast<Transform*>(owner->getComponent(ComponentType::transform));

    if (!transform)
      return;
  }


  falling = true;

  if (doGravity)
    applyForce(gravity);

  limitMovement();

  transform->move({velocity.getX() * dt, velocity.getY() * dt});
}

void RigidBody::applyForce(const Vec2<float>& force)
{
  velocity += force;
}

void RigidBody::limitMovement()
{
  applyForce({-velocity.getX() * 0.2f, 0});
}

bool RigidBody::isFalling() const
{
  return falling;
}

void RigidBody::handleCollision(Vec2<float> minimumTranslationVector)
{
  if (!transform)
  {
    transform = dynamic_cast<Transform*>(owner->getComponent(ComponentType::transform));

    if (!transform)
      return;
  }

  if (minimumTranslationVector.getX() != 0)
    handleXCollision();

  if (minimumTranslationVector.getY() != 0)
    handleYCollision(minimumTranslationVector.getY());

  transform->move(minimumTranslationVector);
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
