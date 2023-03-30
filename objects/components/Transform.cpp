#include "Transform.h"

Transform::Transform(float xPos, float yPos, float width, float height)
    : Component("Transform"), position{xPos, yPos}, oldPosition{position}, initialPosition{position}, w{width}, h{height}
{}

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
    oldPosition = position;
    position.x = xPos;
    position.y = yPos;
}

void Transform::move(float xDif, float yDif)
{
    oldPosition = position;
    position.x += xDif;
    position.y += yDif;
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
}
