#ifndef PLATFORMER_BOXCOLLIDER_H
#define PLATFORMER_BOXCOLLIDER_H

#include "Component.h"
struct BoundingRectangle;

class BoxCollider : public Component
{
public:
    BoxCollider();

    bool collidesWith(BoundingRectangle other);
};

#endif //PLATFORMER_BOXCOLLIDER_H
