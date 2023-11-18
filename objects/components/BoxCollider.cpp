#include "BoxCollider.h"
#include "Transform.h"
#include "../GameObject.h"
#include <cmath>

BoxCollider::BoxCollider()
    : Component{ComponentType::boxCollider}, transform{nullptr}
{}

bool BoxCollider::collidesWith(BoundingRectangle r2)
{
    if (!transform)
        transform = dynamic_cast<Transform*>(owner->getComponent(ComponentType::transform));

    if (!transform)
        return false;

    auto r1 = transform->getBoundingRectangle();

    float mdX =  r1.left - r2.right;
    float mdY = r1.top - r2.bottom;
    float mdW = r1.width() + r2.width();
    float mdH = r1.height() + r2.height();

    return mdX <= 0 && mdX + mdW >= 0 && mdY <= 0 && mdY + mdH >= 0;
}

Vec2<float> BoxCollider::getPenetrationVector(const std::vector<GameObject *>& objects)
{
    Vec2<float> finalPVec = {0, 0};
    float xCollisions = 0;
    float yCollisions = 0;

    for (auto& object : objects)
    {
        auto pVec = getPenetration(object);

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
                auto pVec = BoxCollider::getTheoreticalPenetration(theoreticalRectangle, object);

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
                auto pVec = BoxCollider::getTheoreticalPenetration(theoreticalRectangle, object);

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

    return finalPVec;
}

Vec2<float> BoxCollider::getPenetration(GameObject* object)
{
    if (!transform)
        transform = dynamic_cast<Transform*>(owner->getComponent(ComponentType::transform));

    if (!transform)
        return { 0, 0 };

    auto otherTransform = dynamic_cast<Transform*>(object->getComponent(ComponentType::transform));

    if (!transform || !otherTransform)
        return { 0, 0 };

    auto r1 = transform->getBoundingRectangle();
    auto r2 = otherTransform->getBoundingRectangle();

    return getActualPenetration(r1, r2);
}

Vec2<float> BoxCollider::getTheoreticalPenetration(BoundingRectangle boundingRectangle, GameObject* object)
{
    auto otherTransform = dynamic_cast<Transform*>(object->getComponent(ComponentType::transform));

    if (!otherTransform)
        return { 0, 0 };

    auto r2 = otherTransform->getBoundingRectangle();

    return getActualPenetration(boundingRectangle, r2);
}

Vec2<float> BoxCollider::getActualPenetration(BoundingRectangle r1, BoundingRectangle r2)
{
    float mdX =  r1.left - r2.right;
    float mdY = r1.top - r2.bottom;
    float mdW = r1.width() + r2.width();
    float mdH = r1.height() + r2.height();

    Vec2<float> min{mdX, mdY};
    Vec2<float> max{mdX + mdW, mdY + mdH};

    float minDist = std::fabs(min.getX());
    Vec2<float> pVec{min.getX(), 0};

    if (std::fabs(max.getX()) < minDist)
    {
        minDist = std::fabs(max.getX());
        pVec.setX(max.getX());
    }

    if (std::fabs(min.getY()) < minDist)
    {
        minDist = std::fabs(min.getY());
        pVec.setX(0);
        pVec.setY(min.getY());
    }

    if (std::fabs(max.getY()) < minDist)
    {
        pVec.setX(0);
        pVec.setY(max.getY());
    }

    return pVec;
}
