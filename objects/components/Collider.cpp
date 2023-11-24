#include "Collider.h"
#include "../GameObject.h"
#include "Transform.h"

Collider::Collider()
  : Component{ComponentType::collider}
{}

bool Collider::collidesWith(GameObject* other)
{
  auto transform = dynamic_cast<Transform*>(owner->getComponent(ComponentType::transform));
  if (!transform)
    return false;

  auto otherTransform = dynamic_cast<Transform*>(other->getComponent(ComponentType::transform));
  if (!otherTransform)
    return false;

  auto otherCollider = dynamic_cast<Collider*>(other->getComponent(ComponentType::collider));
  if (!otherCollider)
    return false;

  return false;
}
