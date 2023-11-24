#include "MeshCollider.h"
#include "../GameObject.h"
#include "Transform.h"

Vec2<float> MeshCollider::getSupport(Vec2<float> direction)
{
  auto transform = dynamic_cast<Transform*>(owner->getComponent(ComponentType::transform));

  if (!transform)
    return { 0, 0 };

  auto mesh = transform->getMesh();

  float furthestDistance = -10000000;
  Vec2<float> furthestVertex{ 0, 0 };

  for (auto& vertex : mesh)
  {
    float distance = vertex.dot(direction);

    if (distance > furthestDistance)
    {
      furthestDistance = distance;
      furthestVertex = vertex;
    }
  }

  return furthestVertex;
}
