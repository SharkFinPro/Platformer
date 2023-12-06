#ifndef PLATFORMER_COLLIDER_H
#define PLATFORMER_COLLIDER_H

#include "Component.h"
#include "../../math/Vec2.h"
#include "../../math/Vec3.h"
#include <vector>

class GameObject;

class Collider : public Component
{
public:
  Collider();

  bool collidesWith(GameObject* other);

private:
  virtual Vec3<float> findFurthestPoint(Vec3<float> direction) = 0;

  static Vec3<float> getSupport(Collider* a, Collider* b, Vec3<float> direction);

  static bool nextSimplex(std::vector<Vec3<float>>& simplex, Vec3<float>& direction);

  static bool line(std::vector<Vec3<float>>& simplex, Vec3<float>& direction);
  static bool triangle(std::vector<Vec3<float>>& simplex, Vec3<float>& direction);
};


#endif //PLATFORMER_COLLIDER_H
