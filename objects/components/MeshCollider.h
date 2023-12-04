#ifndef PLATFORMER_MESHCOLLIDER_H
#define PLATFORMER_MESHCOLLIDER_H

#include "Collider.h"

class MeshCollider : public Collider
{
public:
  explicit MeshCollider(bool debug);

private:
  Vec3<float> findFurthestPoint(Vec3<float> direction) override;
};


#endif //PLATFORMER_MESHCOLLIDER_H
