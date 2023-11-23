#ifndef PLATFORMER_TRANSFORM_H
#define PLATFORMER_TRANSFORM_H

#include "Component.h"
#include "../../math/Vec2.h"

struct BoundingRectangle {
  float left, right, top, bottom;

  float width() const {
    return right - left;
  }

  float height() const {
    return bottom - top;
  }
};

class Transform : public Component
{
public:
  Transform(float xPos, float yPos, float width, float height);

  void fixedUpdate(float dt) override;

  float getX() const;
  float getY() const;

  void move(float xDif, float yDif);

  float getWidth() const;
  float getHeight() const;

  BoundingRectangle getBoundingRectangle() const;

  void reset();

private:
  Vec2<float> position;
  Vec2<float> oldPosition;
  Vec2<float> initialPosition;
  Vec2<float> newPosition;

  float w, h;
};


#endif //PLATFORMER_TRANSFORM_H
