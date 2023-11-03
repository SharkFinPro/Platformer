#include "RigidBody.h"
#include "../GameObject.h"
#include "BoxCollider.h"
#include "Transform.h"
#include <cmath>

RigidBody::RigidBody()
    : Component{ComponentType::rigidBody}, xvel{0}, yvel{0}, doGravity{true}, gravity{0.4f}, maxFallSpeed{15}, falling{true}, collided{false}
{}

void RigidBody::fixedUpdate(float dt)
{
    auto transform = dynamic_cast<Transform*>(owner->getComponent(ComponentType::transform));

    if (!transform)
        return;

    falling = true;


    if (doGravity)
        yvel += gravity * dt;

    limitMovement();

    transform->move(xvel * dt, yvel * dt);

    collided = false;
}

void RigidBody::limitMovement()
{
    xvel /= 1.01f;

    if (yvel > maxFallSpeed)
        yvel = maxFallSpeed;
}

void RigidBody::setXvel(float velocity)
{
    if (collided)
        return;

    xvel = velocity;
}

void RigidBody::setYvel(float velocity)
{
    yvel = velocity;
}

bool RigidBody::isFalling() const
{
    return falling;
}

void RigidBody::handleCollision(GameObject* other, float dt)
{
    auto transform = dynamic_cast<Transform*>(owner->getComponent(ComponentType::transform));
    auto otherTransform = dynamic_cast<Transform*>(other->getComponent(ComponentType::transform));

    if (!transform || !otherTransform)
        return;

    auto r1 = transform->getBoundingRectangle();
    auto r2 = otherTransform->getBoundingRectangle();

    float mdX =  r1.left - r2.right;
    float mdY = r1.top - r2.bottom;
    float mdW = r1.width() + r2.width();
    float mdH = r1.height() + r2.height();

    float minX = mdX;
    float minY = mdY;
    float maxX = mdX + mdW;
    float maxY = mdY + mdH;

    float minDist = std::fabs(minX);
    float vecX = minX;
    float vecY = 0;

    if (std::fabs(maxX) < minDist)
    {
        minDist = std::fabs(maxX);
        vecX = maxX;
    }

    if (std::fabs(minY) < minDist)
    {
        minDist = std::fabs(minY);
        vecX = 0;
        vecY = minY;
    }

    if (std::fabs(maxY) < minDist)
    {
        vecX = 0;
        vecY = maxY;
    }

    if (vecY == maxY)
    {
        yvel = -vecY;
        falling = false;
    }
    else if (vecY == minY)
        yvel = gravity;

    if (vecX != 0)
    {
        transform->setX(transform->getX() - vecX);
        collided = true;
    }
}
