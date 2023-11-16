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
    static Vec2<float> getTheoreticalPenetrationVector(BoundingRectangle boundingRectangle, GameObject* object);

private:
    static Vec2<float> getActualPenetrationVector(BoundingRectangle r1, BoundingRectangle r2);
};

#endif //PLATFORMER_BOXCOLLIDER_H
