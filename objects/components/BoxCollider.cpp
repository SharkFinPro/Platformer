#include "BoxCollider.h"
#include "Transform.h"
#include "../GameObject.h"

BoxCollider::BoxCollider()
    : Component{ComponentType::boxCollider}
{}

bool BoxCollider::collidesWith(BoundingRectangle r2)
{
    auto transform = dynamic_cast<Transform*>(owner->getComponent(ComponentType::transform));

    if (!transform)
        return false;

    auto r1 = transform->getBoundingRectangle();

    return !(r1.bottom < r2.top || r1.top > r2.bottom || r1.right < r2.left || r1.left > r2.right);
}
