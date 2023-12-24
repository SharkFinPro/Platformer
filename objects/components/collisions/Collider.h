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

  bool collidesWith(GameObject* other, std::vector<Vec3<float>>& polytope, Vec3<float> translation = {0, 0, 0});

  Vec2<float> getPenetrationVector(std::vector<std::pair<GameObject*, std::vector<Vec3<float>>>>& collisions);

protected:

  Transform* transform;
private:

  virtual Vec3<float> findFurthestPoint(Vec3<float> direction, Vec3<float> translation) = 0;
  static Vec3<float> getSupport(Collider* a, Collider* b, Vec3<float> direction, Vec3<float> translation);

  static bool nextSimplex(Simplex& simplex, Vec3<float>& direction);

  static bool line(Simplex& simplex, Vec3<float>& direction);

  static bool triangle(Simplex& simplex, Vec3<float>& direction);

  static Vec3<float> getClosestPointOnLine(Vec3<float> a, Vec3<float> b, Vec3<float> c);

  Vec3<float> EPA(std::vector<Vec3<float>>& polytope, GameObject* other, Vec3<float> translation);
};


#endif //PLATFORMER_COLLIDER_H
