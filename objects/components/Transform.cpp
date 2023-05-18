#include "Transform.h"

Transform::Transform(float xPos, float yPos, float width, float height)
    : Component(ComponentType::transform), position{xPos, yPos}, oldPosition{position}, initialPosition{position}, w{width}, h{height}, newPosition{xPos, yPos}
{}

void Transform::fixedUpdate(float dt) {
    oldPosition.x = position.x;
    oldPosition.y = position.y;

    position.x = newPosition.x;
    position.y = newPosition.y;
}

float Transform::getX() const
{
    return position.x;
}

float Transform::getY() const
{
    return position.y;
}

void Transform::setPosition(float xPos, float yPos)
{
    newPosition.x = xPos;
    newPosition.y = yPos;
}

void Transform::setX(float xPos)
{
    newPosition.x = xPos;
}

void Transform::setY(float yPos)
{
    newPosition.y = yPos;
}

void Transform::move(float xDif, float yDif)
{
    newPosition.x += xDif;
    newPosition.y += yDif;
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
    return BoundingRectangle{position.x, position.x + w, position.y, position.y + h};
}

BoundingRectangle Transform::getPastBoundingRectangle() const
{
    return BoundingRectangle{oldPosition.x, oldPosition.x + w, oldPosition.y, oldPosition.y + h};
}

void Transform::reset()
{
    oldPosition = position;
    position = initialPosition;
    newPosition = initialPosition;
}
