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

  int rep = 0;
  for (auto& vertex : mesh)
  {
    Vec3<float> vert = { vertex.getX(), vertex.getY(), 0 };
    float distance = vert.dot(direction);

    if (distance > furthestDistance)
    {
      furthestDistance = distance;
      furthestVertex = vert;
      rep++;
    }
  }

  return furthestVertex;
}
