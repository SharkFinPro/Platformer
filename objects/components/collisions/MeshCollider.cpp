#include "MeshCollider.h"
#include "../../GameObject.h"
#include "../Transform.h"
#include <cfloat>

Vec3<float> MeshCollider::findFurthestPoint(Vec3<float> direction)
{
  auto transform = dynamic_cast<Transform*>(owner->getComponent(ComponentType::transform));

  if (!transform)
    return { 0, 0, 0 };

  auto mesh = transform->getMesh();

  float furthestDistance = -FLT_MAX;
  Vec3<float> furthestVertex;

  float distance;
  Vec3<float> vert;

  for (auto& vertex : mesh)
  {
    vert = {vertex, 0};
    distance = vert.dot(direction);

    if (distance > furthestDistance)
    {
      furthestDistance = distance;
      furthestVertex = vert;
    }
  }

  return furthestVertex;
}
