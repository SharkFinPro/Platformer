#include "RigidBody.h"
#include "../GameObject.h"
#include "Transform.h"
#include <cmath>

RigidBody::RigidBody()
    : Component{ComponentType::rigidBody}, xvel{0}, yvel{0}, doGravity{true}, gravity{0.01f}, falling{true}, transform{nullptr}
{}

void RigidBody::fixedUpdate(float dt)
{
    if (!transform)
        transform = dynamic_cast<Transform*>(owner->getComponent(ComponentType::transform));

    if (!transform)
        return;

    falling = true;

    if (doGravity)
        yvel += gravity;

    limitMovement();

    transform->move(xvel * dt, yvel * dt);
}

void RigidBody::applyForce(float x, float y)
{
    xvel += x;
    yvel += y;
}

void RigidBody::limitMovement()
{
    xvel /= 1.5f;
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

    transform->move(-penetrationVector.getX(), -penetrationVector.getY());
}

void RigidBody::handleXCollision()
{
    xvel = 0;
}

void RigidBody::handleYCollision()
{
    if (yvel > 0)
        falling = false;

    yvel = 0;
}
