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
    float xCollisions = 0;
    float yCollisions = 0;

    for (auto& object : objects)
    {
        auto pVec = collider->getPenetrationVector(object);

        if (pVec.getX() != 0)
        {
            xCollisions++;
            finalPVec.setX(finalPVec.getX() + pVec.getX());
        }

        if (pVec.getY() != 0)
        {
            yCollisions++;
            finalPVec.setY(finalPVec.getY() + pVec.getY());
        }
    }

    if (xCollisions != 0)
        finalPVec.setX(finalPVec.getX() / xCollisions);

    if (yCollisions != 0)
        finalPVec.setY(finalPVec.getY() / yCollisions);

    if (finalPVec.getX() != 0 && finalPVec.getY() != 0)
    {
        if (std::fabs(finalPVec.getX()) > std::fabs(finalPVec.getY()))
        {
            auto theoreticalRectangle = transform->getBoundingRectangle();
            theoreticalRectangle.left -= finalPVec.getX();
            theoreticalRectangle.right -= finalPVec.getX();

            transform->move(-finalPVec.getX(), 0);
            handleXCollision();

            finalPVec.setX(0);
            finalPVec.setY(0);

            yCollisions = 0;
            for (auto& object : objects)
            {
                auto pVec = BoxCollider::getTheoreticalPenetrationVector(theoreticalRectangle, object);

                if (pVec.getY() != 0)
                {
                    yCollisions++;
                    finalPVec.setY(finalPVec.getY() + pVec.getY());
                }
            }

            if (finalPVec.getY() != 0)
            {
                handleYCollision();
                transform->move(0, -(finalPVec.getY() / yCollisions));
            }
        }
        else
        {
            auto theoreticalRectangle = transform->getBoundingRectangle();
            theoreticalRectangle.top -= finalPVec.getY();
            theoreticalRectangle.bottom -= finalPVec.getY();

            transform->move(0, -finalPVec.getY());
            handleYCollision();

            finalPVec.setX(0);
            finalPVec.setY(0);

            xCollisions = 0;
            for (auto& object : objects)
            {
                auto pVec = BoxCollider::getTheoreticalPenetrationVector(theoreticalRectangle, object);

                if (pVec.getX() != 0)
                {
                    xCollisions++;
                    finalPVec.setX(finalPVec.getX() + pVec.getX());
                }
            }

            if (finalPVec.getX() != 0)
            {
                handleXCollision();
                transform->move(-(finalPVec.getX() / xCollisions), 0);
            }
        }
    }
    else
    {
        transform->move(-finalPVec.getX(), -finalPVec.getY());

        if (finalPVec.getX() != 0)
            handleXCollision();

        if (finalPVec.getY() != 0)
            handleYCollision();
    }
}

void RigidBody::handleXCollision()
{
    xvel = 0;
}

void RigidBody::handleYCollision()
{
    if (yvel > 0)
    {
        falling = false;
        yvel = 0;
    }
    else
        yvel = gravity;
}
