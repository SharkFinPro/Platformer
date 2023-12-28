#include "Transform.h"
#include <utility>

Transform::Transform(float xPos, float yPos, std::vector<Vec3<float>> mesh)
  : Component(ComponentType::transform), initialPosition{xPos, yPos, 0}, position{initialPosition}, newPosition{initialPosition}, mesh{std::move(mesh)}
{}

void Transform::fixedUpdate([[maybe_unused]] float dt)
{
  position = newPosition;
}

Vec3<float> Transform::getPosition() const
{
  return position;
}

void Transform::move(Vec3<float> vector)
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
    transformedMesh.emplace_back(m.getX() + position.getX(), m.getY() + position.getY(), m.getZ() + position.getZ());

  return transformedMesh;
}
