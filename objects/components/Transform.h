#ifndef PLATFORMER_TRANSFORM_H
#define PLATFORMER_TRANSFORM_H

#include "Component.h"

struct BoundingRectangle {
    float left, right, top, bottom;
};

struct vec2 {
    float x, y;
};

class Transform : public Component
{
public:
    Transform(float xPos, float yPos, float width, float height);

    void fixedUpdate(float dt) override;

    float getX() const;
    float getY() const;

    void setPosition(float xPos, float yPos);
    void setX(float xPos);
    void setY(float yPos);
    void move(float xDif, float yDif);

    float getWidth() const;
    float getHeight() const;

    BoundingRectangle getBoundingRectangle() const;
    BoundingRectangle getPastBoundingRectangle() const;

    void reset();

private:
    vec2 position;
    vec2 oldPosition;
    vec2 initialPosition;
    vec2 newPosition;

    float w, h;
};


#endif //PLATFORMER_TRANSFORM_H
