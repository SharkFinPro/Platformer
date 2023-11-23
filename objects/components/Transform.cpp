#include "Transform.h"

Transform::Transform(float xPos, float yPos, float width, float height)
  : Component(ComponentType::transform), position{xPos, yPos}, oldPosition{position}, initialPosition{position}, newPosition{xPos, yPos}, w{width}, h{height}
{}

void Transform::fixedUpdate([[maybe_unused]] float dt) {
  oldPosition = position;
  position = newPosition;
}

float Transform::getX() const
{
  return position.getX();
}

float Transform::getY() const
{
  return position.getY();
}

void Transform::move(float xDif, float yDif)
{
  newPosition.setX(newPosition.getX() + xDif);
  newPosition.setY(newPosition.getY() + yDif);
}

float Transform::getWidth() const
{
  return w;
}

float Transform::getHeight() const
{
  return h;
}

BoundingRectangle Transform::getBoundingRectangle() const
{
  return BoundingRectangle{position.getX(), position.getX() + w, position.getY(), position.getY() + h};
}

void Transform::reset()
{
  oldPosition = position;
  position = initialPosition;
  newPosition = initialPosition;
}
