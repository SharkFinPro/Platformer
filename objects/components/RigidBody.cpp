#include "RigidBody.h"
#include "../GameObject.h"
#include "BoxCollider.h"
#include "Transform.h"
#include <cmath>

RigidBody::RigidBody()
    : Component{ComponentType::rigidBody}, xvel{0}, yvel{0}, doGravity{true}, gravity{0.01f}, falling{true}, transform{
    nullptr}, boxCollider{nullptr}
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

void RigidBody::handleCollisions(const std::vector<GameObject*>& objects)
{
    if (!transform)
        transform = dynamic_cast<Transform*>(owner->getComponent(ComponentType::transform));

    if (!transform)
        return;

    if (!boxCollider)
        boxCollider = dynamic_cast<BoxCollider*>(owner->getComponent(ComponentType::boxCollider));

    if (!boxCollider)
        return;

    Vec2<float> finalPVec = {0, 0};
    float xCollisions = 0;
    float yCollisions = 0;

    for (auto& object : objects)
    {
        auto pVec = boxCollider->getPenetrationVector(object);

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

    if (finalPVec.getX() != 0 && finalPVec.getY() != 0)
    {
        if (std::fabs(finalPVec.getX()) > std::fabs(finalPVec.getY()))
        {
            auto theoreticalRectangle = transform->getBoundingRectangle();
            theoreticalRectangle.left -= finalPVec.getX();
            theoreticalRectangle.right -= finalPVec.getX();

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
        }
        else
        {
            auto theoreticalRectangle = transform->getBoundingRectangle();
            theoreticalRectangle.top -= finalPVec.getY();
            theoreticalRectangle.bottom -= finalPVec.getY();

            finalPVec.setX(0);

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
        }
    }

    if (xCollisions != 0)
        finalPVec.setX(finalPVec.getX() / xCollisions);

    if (yCollisions != 0)
        finalPVec.setY(finalPVec.getY() / yCollisions);

    if (finalPVec.getX() != 0)
        handleXCollision();

    if (finalPVec.getY() != 0)
        handleYCollision();

    transform->move(-finalPVec.getX(), -finalPVec.getY());
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
