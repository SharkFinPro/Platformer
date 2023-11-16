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

Vec2<float> BoxCollider::getPenetrationVector(GameObject* object)
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

    return getActualPenetrationVector(r1, r2);
}

Vec2<float> BoxCollider::getTheoreticalPenetrationVector(BoundingRectangle boundingRectangle, GameObject* object)
{
    auto otherTransform = dynamic_cast<Transform*>(object->getComponent(ComponentType::transform));

    if (!otherTransform)
        return { 0, 0 };

    auto r2 = otherTransform->getBoundingRectangle();

    return getActualPenetrationVector(boundingRectangle, r2);
}

Vec2<float> BoxCollider::getActualPenetrationVector(BoundingRectangle r1, BoundingRectangle r2)
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
