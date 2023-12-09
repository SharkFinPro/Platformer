#ifndef PLATFORMER_MESHCOLLIDER_H
#define PLATFORMER_MESHCOLLIDER_H

#include "Collider.h"

class Transform;

class MeshCollider : public Collider
{
public:
  MeshCollider();

private:
  Vec3<float> findFurthestPoint(Vec3<float> direction) override;

  Transform* transform;
};


#endif //PLATFORMER_MESHCOLLIDER_H
