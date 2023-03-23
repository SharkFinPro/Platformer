#include "RigidBody.h"
#include "../GameObject.h"
#include "BoxCollider.h"
#include "Transform.h"

RigidBody::RigidBody()
    : Component{"RigidBody"}, xvel{0}, yvel{0}, doGravity{true}, gravity{0.4f}, maxFallSpeed{15}, falling{true}, collided{false}
{}

void RigidBody::fixedUpdate(float dt)
{
    auto transform = dynamic_cast<Transform*>(owner->getComponent("Transform"));

    if (!transform)
        return;

    if (!collided)
        falling = true;

    transform->move(xvel * dt, yvel * dt);

    if (doGravity)
        yvel += gravity * dt;

    limitMovement();

    collided = false;
}

void RigidBody::limitMovement()
{
    xvel /= 2.0f;

    if (yvel > maxFallSpeed)
        yvel = maxFallSpeed;
}

float RigidBody::getXvel() const
{
    return xvel;
}

float RigidBody::getYvel() const
{
    return yvel;
}

void RigidBody::setXvel(float velocity)
{
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
    auto transform = dynamic_cast<Transform*>(owner->getComponent("Transform"));
    auto otherTransform = dynamic_cast<Transform*>(other->getComponent("Transform"));
    auto otherRb = dynamic_cast<RigidBody*>(other->getComponent("RigidBody"));

    if (!transform || !otherTransform)
        return;

    auto r1 = transform->getBoundingRectangle();
    auto pr1 = transform->getPastBoundingRectangle();
    auto r2 = otherTransform->getBoundingRectangle();
    auto pr2 = otherTransform->getPastBoundingRectangle();

    if (r1.bottom >= r2.top && pr1.bottom < pr2.top)
    {
        transform->setPosition(transform->getX(), r2.top - transform->getHeight() - 0.001f);
        falling = false;
        yvel = 0;
    }
    else if (r1.top <= r2.bottom && pr1.top > pr2.bottom)
    {
        transform->setPosition(transform->getX(), pr2.bottom + 0.001f);
        yvel = 0;
    }
    else if (r1.right >= r2.left && pr1.right < pr2.left)
    {
        transform->setPosition(r2.left - transform->getWidth() - 0.001f, transform->getY());

        if (otherRb)
        {
            xvel = otherRb->xvel + xvel;
            otherRb->xvel = xvel + otherRb->xvel;
        }
        else
            xvel = 0;
    }
    else if (r1.left <= r2.right && pr1.left > pr2.right)
    {
        transform->setPosition(r2.right + 0.001f, transform->getY());

        if (otherRb)
        {
            xvel = otherRb->xvel + xvel;
            otherRb->xvel = xvel + otherRb->xvel;
        }
        else
            xvel = 0;
    }

    collided = true;
}
