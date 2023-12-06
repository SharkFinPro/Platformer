#include "RigidBody.h"
#include "../GameObject.h"
#include "Transform.h"

RigidBody::RigidBody()
  : Component{ComponentType::rigidBody}, velocity{0, 0}, doGravity{true}, gravity{0.4f}, falling{true}, transform{nullptr}
{}

void RigidBody::fixedUpdate(float dt)
{
  if (!transform)
    transform = dynamic_cast<Transform*>(owner->getComponent(ComponentType::transform));

  if (!transform)
    return;

  falling = true;

  if (doGravity)
    velocity += { 0.0f, gravity };

  limitMovement();

  transform->move({velocity.getX() * dt, velocity.getY() * dt});
}

void RigidBody::applyForce(Vec2<float> force)
{
  velocity += force;
}

void RigidBody::limitMovement()
{
  velocity.setX(velocity.getX() / 1.4f);
}

bool RigidBody::isFalling() const
{
  return falling;
}

void RigidBody::handleCollision(Vec2<float> penetrationVector)
{
  if (!transform)
    transform = dynamic_cast<Transform*>(owner->getComponent(ComponentType::transform));

  if (!transform)
    return;

  if (penetrationVector.getX() != 0)
    handleXCollision();

  if (penetrationVector.getY() != 0)
    handleYCollision();

  transform->move({-penetrationVector.getX(), -penetrationVector.getY()});
}

void RigidBody::handleXCollision()
{
  velocity.setX(0);
}

void RigidBody::handleYCollision()
{
  if (velocity.getY() > 0)
    falling = false;

  velocity.setY(0);
}
