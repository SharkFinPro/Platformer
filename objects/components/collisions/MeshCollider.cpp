#include "MeshCollider.h"
#include "../../Object.h"
#include "../Transform.h"
#include <cfloat>
#include <stdexcept>

Vec3<float> MeshCollider::findFurthestPoint(const Vec3<float>& direction)
{
  if (transform_ptr.expired())
  {
    transform_ptr = std::dynamic_pointer_cast<Transform>(owner->getComponent(ComponentType::transform));

    if (transform_ptr.expired())
    {
      throw std::runtime_error("MeshCollider::findFurthestPoint::Missing transform component");
    }
  }

  float furthestDistance = -FLT_MAX;
  Vec3<float> furthestVertex;

  if (const std::shared_ptr<Transform> transform = transform_ptr.lock())
  {
    for (auto& vertex : transform->getMesh())
    {
      if (const float distance = vertex.dot(direction); distance > furthestDistance)
      {
        furthestDistance = distance;
        furthestVertex = vertex;
      }
    }
  }

  return furthestVertex;
}
