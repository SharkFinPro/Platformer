#ifndef PLATFORMER_BOXCOLLIDER_H
#define PLATFORMER_BOXCOLLIDER_H

#include "Component.h"
#include "../../math/Vec2.h"

struct BoundingRectangle;
class GameObject;

class BoxCollider : public Component
{
public:
    BoxCollider();

    bool collidesWith(BoundingRectangle other);

    Vec2<float> getPenetrationVector(GameObject* object) const;
};

#endif //PLATFORMER_BOXCOLLIDER_H
