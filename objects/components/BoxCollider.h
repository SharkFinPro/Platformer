#ifndef PLATFORMER_BOXCOLLIDER_H
#define PLATFORMER_BOXCOLLIDER_H

#include "Component.h"
class Transform;

class BoxCollider : public Component
{
public:
    BoxCollider();

    bool collidesWith(Transform* other);
};

#endif //PLATFORMER_BOXCOLLIDER_H
