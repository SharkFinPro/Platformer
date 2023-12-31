#ifndef PLATFORMER_COLLIDER_H
#define PLATFORMER_COLLIDER_H

#include "../Component.h"
#include "../../../math/Vec3.h"
#include "Simplex.h"
#include <vector>
#include <memory>

class Object;
class Transform;

class Collider : public Component
{
public:
  Collider();

  bool collidesWith(const std::shared_ptr<Object>& other, std::vector<Vec3<float>>& polytope, Vec3<float> translation = {0});

  Vec3<float> minimumTranslationVector(std::vector<std::pair<std::shared_ptr<Object>, std::vector<Vec3<float>>>>& collisions);

protected:
  std::weak_ptr<Transform> transform_ptr;

private:

  virtual Vec3<float> findFurthestPoint(Vec3<float> direction, Vec3<float> translation) = 0;
  Vec3<float> getSupport(const std::shared_ptr<Collider>& b, Vec3<float> direction, Vec3<float> translation);

  static bool nextSimplex(Simplex& simplex, Vec3<float>& direction);

  static bool line(Simplex& simplex, Vec3<float>& direction);

  static bool triangle(Simplex& simplex, Vec3<float>& direction);

  static Vec3<float> getClosestPointOnLine(Vec3<float> a, Vec3<float> b, Vec3<float> c);

  Vec3<float> EPA(std::vector<Vec3<float>>& polytope, const std::shared_ptr<Object>& other, Vec3<float> translation);
};


#endif //PLATFORMER_COLLIDER_H
