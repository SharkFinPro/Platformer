#include "Transform.h"
#include <utility>

Transform::Transform(float xPos, float yPos, std::vector<Vec2<float>> mesh)
  : Component(ComponentType::transform), initialPosition{xPos, yPos}, position{initialPosition}, newPosition{initialPosition}, mesh{std::move(mesh)}
{}

void Transform::fixedUpdate([[maybe_unused]] float dt)
{
  position = newPosition;
}

Vec2<float> Transform::getPosition() const
{
  return position;
}

void Transform::move(Vec2<float> vector)
{
  newPosition += vector;
}

void Transform::reset()
{
  position = initialPosition;
  newPosition = initialPosition;
}

std::vector<Vec2<float>> Transform::getMesh() const {
  std::vector<Vec2<float>> transformedMesh;

  for (auto& m : mesh)
    transformedMesh.emplace_back(m.getX() + position.getX(), m.getY() + position.getY());

  return transformedMesh;
}
