#include "BoxCollider.h"
#include "Transform.h"
#include "../GameObject.h"

BoxCollider::BoxCollider()
    : Component{"BoxCollider"}
{}

bool BoxCollider::collidesWith(Transform* other)
{
    Transform* transform = dynamic_cast<Transform*>(owner->getComponent("Transform"));

    if (!transform || !other)
        return false;

    BoundingRectangle r1 = transform->getBoundingRectangle();
    BoundingRectangle r2 = other->getBoundingRectangle();

    return !(r1.bottom < r2.top || r1.top > r2.bottom || r1.right < r2.left || r1.left > r2.right);
}
