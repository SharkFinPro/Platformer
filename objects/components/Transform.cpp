#include "Transform.h"
#include <utility>

Transform::Transform(float x, float y, std::vector<Vec3<float>> mesh)
  : Component(ComponentType::transform), initialPosition{x, y, 0}, position{initialPosition}, newPosition{initialPosition}, mesh{std::move(mesh)}
{}

void Transform::fixedUpdate([[maybe_unused]] float dt)
{
  position = newPosition;
}

Vec3<float> Transform::getPosition() const
{
  return position;
}

void Transform::move(const Vec3<float>& vector)
{
  newPosition += vector;
}

void Transform::reset()
{
  position = initialPosition;
  newPosition = initialPosition;
}

std::vector<Vec3<float>> Transform::getMesh() const {
  std::vector<Vec3<float>> transformedMesh;

  for (auto& m : mesh)
    transformedMesh.emplace_back(m + position);

  return transformedMesh;
}
