#ifndef PLATFORMER_COLLIDER_H
#define PLATFORMER_COLLIDER_H

#include "../Component.h"
#include "../../../math/Vec3.h"
#include "Simplex.h"
#include <vector>
#include <memory>

class Object;
class Transform;

typedef std::vector<Vec3<float>> Polytope;

class Collider : public Component
{
public:
  Collider();

  bool collidesWith(const std::shared_ptr<Object>& other, Vec3<float>* mtv);

  static Vec3<float> closestPointOnLine(Vec3<float> a, Vec3<float> b, Vec3<float> c);

protected:
  std::weak_ptr<Transform> transform_ptr;

private:
  virtual Vec3<float> findFurthestPoint(Vec3<float> direction) = 0;
  Vec3<float> getSupport(const std::shared_ptr<Collider>& b, Vec3<float> direction);

  static bool expandSimplex(Simplex& simplex, Vec3<float>& direction);

  static bool lineCase(Simplex& simplex, Vec3<float>& direction);

  static bool triangleCase(Simplex& simplex, Vec3<float>& direction);

  Vec3<float> EPA(Polytope& polytope, const std::shared_ptr<Object>& other);
};


#endif //PLATFORMER_COLLIDER_H
