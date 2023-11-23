#include "Transform.h"

Transform::Transform(double xPos, double yPos, double width, double height)
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

void Transform::fixedUpdate([[maybe_unused]] double dt) {
  oldMesh = mesh;
  mesh = newMesh;
}

double Transform::getX() const
{
  return mesh.at(0).getX();
}

double Transform::getY() const
{
  return mesh.at(0).getY();
}

void Transform::move(Vec2<double> vector)
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

std::vector<Vec2<double>> Transform::getMesh() const {
  return mesh;
}
