#ifndef PLATFORMER_MESHCOLLIDER_H
#define PLATFORMER_MESHCOLLIDER_H

#include "Collider.h"

class MeshCollider : public Collider
{
private:
  Vec2<float> getSupport(Vec2<float> direction) override;
};


#endif //PLATFORMER_MESHCOLLIDER_H
