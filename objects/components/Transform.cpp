#include "Transform.h"

Transform::Transform(float xPos, float yPos, float width, float height)
  : Component(ComponentType::transform)
{
  initialMesh.push_back({xPos, yPos});
  initialMesh.push_back({xPos + width, yPos});
  initialMesh.push_back({xPos + width, yPos + height});
  initialMesh.push_back({xPos, yPos + height});

  oldMesh = initialMesh;
  mesh = initialMesh;
  newMesh = initialMesh;
}

void Transform::fixedUpdate([[maybe_unused]] float dt) {
  oldMesh = mesh;
  mesh = newMesh;
}

float Transform::getX() const
{
  return mesh.at(0).getX();
}

float Transform::getY() const
{
  return mesh.at(0).getY();
}

void Transform::move(Vec2<float> vector)
{
  for (auto& m : newMesh)
  {
    m.setX(m.getX() + vector.getX());
    m.setY(m.getY() + vector.getY());
  }
}

void Transform::reset()
{
  oldMesh = initialMesh;
  mesh = initialMesh;
  newMesh = mesh;
}

std::vector<Vec2<float>> Transform::getMesh() const {
  return mesh;
}
