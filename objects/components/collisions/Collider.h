#ifndef PLATFORMER_COLLIDER_H
#define PLATFORMER_COLLIDER_H

#include "../Component.h"
#include "../../../math/Vec2.h"
#include "../../../math/Vec3.h"
#include <vector>
#include "Simplex.h"

class GameObject;
class Transform;

class Collider : public Component
{
public:
  Collider();

  bool collidesWith(GameObject* other);

protected:
  Transform* transform;

private:
  virtual Vec3<float> findFurthestPoint(Vec3<float> direction) = 0;

  static Vec3<float> getSupport(Collider* a, Collider* b, Vec3<float>& direction);

  static bool nextSimplex(Simplex& simplex, Vec3<float>& direction);

  static bool line(Simplex& simplex, Vec3<float>& direction);
  static bool triangle(Simplex& simplex, Vec3<float>& direction);
};


#endif //PLATFORMER_COLLIDER_H
