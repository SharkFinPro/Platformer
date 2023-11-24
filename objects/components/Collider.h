#ifndef PLATFORMER_COLLIDER_H
#define PLATFORMER_COLLIDER_H

#include "Component.h"
#include "../../math/Vec2.h"

class GameObject;

class Collider : public Component
{
public:
  Collider();

  bool collidesWith(GameObject* other);

private:
  virtual Vec2<float> getSupport(Vec2<float> direction) = 0;
};


#endif //PLATFORMER_COLLIDER_H
