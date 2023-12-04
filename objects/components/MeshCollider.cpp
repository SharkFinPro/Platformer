#include "MeshCollider.h"
#include "../GameObject.h"
#include "Transform.h"
#include <cfloat>
#include <iostream>

MeshCollider::MeshCollider(bool debug)
  : Collider(debug)
{}

Vec3<float> MeshCollider::findFurthestPoint(Vec3<float> direction)
{
  auto transform = dynamic_cast<Transform*>(owner->getComponent(ComponentType::transform));

  if (!transform)
    return { 0, 0, 0 };

  auto mesh = transform->getMesh();

  float furthestDistance = -FLT_MAX;
  Vec3<float> furthestVertex{ 0, 0, 0 };

  float distance;
  Vec3<float> vert = {0, 0, 0};

  for (auto& vertex : mesh)
  {
    vert = { vertex.getX(), vertex.getY(), 0 };
    distance = vert.dot(direction);

    if (distance > furthestDistance)
    {
      furthestDistance = distance;
      furthestVertex = vert;
    }
  }

  return furthestVertex;
}
