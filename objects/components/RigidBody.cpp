#include "RigidBody.h"
#include "../GameObject.h"
#include "BoxCollider.h"
#include "Transform.h"
#include <cmath>

RigidBody::RigidBody()
    : Component{ComponentType::rigidBody}, xvel{0}, yvel{0}, doGravity{true}, gravity{0.4f}, maxFallSpeed{15}, falling{true}
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
}

void RigidBody::limitMovement()
{
    xvel /= 1.01f;

    if (yvel > maxFallSpeed)
        yvel = maxFallSpeed;
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

void RigidBody::handleCollisions(const std::vector<GameObject*>& objects)
{
    auto transform = dynamic_cast<Transform*>(owner->getComponent(ComponentType::transform));
    auto collider = dynamic_cast<BoxCollider*>(owner->getComponent(ComponentType::boxCollider));

    if (!transform || !collider)
        return;

    Vec2<float> finalPVec = {0, 0};

    for (auto& object : objects)
    {
        auto pVec = collider->getPenetrationVector(object);

        finalPVec.setX(finalPVec.getX() + pVec.getX());
        finalPVec.setY(finalPVec.getY() + pVec.getY());
    }

    if (finalPVec.getX() != 0)
    {
        xvel = 0;
    }

    if (finalPVec.getY() != 0)
    {
        if (yvel > 0)
            falling = false;

        yvel = 0;
    }

    transform->move(-finalPVec.getX(), -finalPVec.getY());
}
