#include "MeshCollider.h"
#include "../../GameObject.h"
#include "../Transform.h"
#include <cfloat>

Vec3<float> MeshCollider::findFurthestPoint(Vec3<float> direction, Vec3<float> translation)
{
  if (!transform)
  {
    transform = dynamic_cast<Transform*>(owner->getComponent(ComponentType::transform));

    if (!transform)
      throw std::runtime_error("Cannot use a collider without a transform");
  }

  float furthestDistance = -FLT_MAX, distance;
  Vec3<float> furthestVertex, vert;

  for (auto& vertex : transform->getMesh())
  {
    vert = Vec3<float>(vertex, 0) + translation;
    distance = vert.dot(direction);

    if (distance > furthestDistance)
    {
      furthestDistance = distance;
      furthestVertex = vert;
    }
  }

  return furthestVertex;
}
