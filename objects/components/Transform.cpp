#include "Transform.h"
#include <utility>

Transform::Transform(float x, float y, std::vector<Vec3<float>> mesh)
  : Component(ComponentType::transform), initialPosition{x, y, 0}, position{initialPosition}, mesh{std::move(mesh)}
{}

Vec3<float> Transform::getPosition() const
{
  return position;
}

void Transform::move(const Vec3<float>& vector)
{
  position += vector;
}

void Transform::reset()
{
  position = initialPosition;
}

std::vector<Vec3<float>> Transform::getMesh() const
{
  std::vector<Vec3<float>> transformedMesh;
  transformedMesh.reserve(mesh.size());

  for (const auto& m : mesh)
  {
    transformedMesh.emplace_back(m + position);
  }

  return transformedMesh;
}
