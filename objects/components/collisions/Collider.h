#ifndef PLATFORMER_COLLIDER_H
#define PLATFORMER_COLLIDER_H

#include "../Component.h"
#include "../../../math/Vec3.h"
#include "Simplex.h"
#include <vector>
#include <memory>
#include <optional>

class Object;
class Transform;

typedef std::vector<Vec3<float>> Polytope;

struct ClosestEdgeData {
  Vec3<float> closestPoint;
  int closestIndex = -1;
  Vec3<float> a;
  Vec3<float> b;
};

class Collider : public Component
{
public:
  Collider();

  bool collidesWith(const std::shared_ptr<Object>& other, Vec3<float>* mtv);

private:

  Vec3<float> getSupport(const std::shared_ptr<Collider>& b, const Vec3<float>& direction);
  virtual Vec3<float> findFurthestPoint(const Vec3<float>& direction) = 0;

  static bool expandSimplex(Simplex& simplex, Vec3<float>& direction);
  static bool lineCase(Simplex& simplex, Vec3<float>& direction);
  static bool triangleCase(Simplex& simplex, Vec3<float>& direction);

  Vec3<float> EPA(Polytope& polytope, const std::shared_ptr<Object>& other);
  static Vec3<float> closestPointOnLine(const Vec3<float>& a, const Vec3<float>& b, const Vec3<float>& c);
  static float findClosestEdge(const Polytope& polytope, ClosestEdgeData& closestEdgeData);
  static bool closeEnough(float minDistance, const std::optional<float>& previousMinDistance, const Vec3<float>& currentClosestPoint, const std::optional<Vec3<float>>& previousClosestPoint);
  static Vec3<float> getSearchDirection(const ClosestEdgeData& closestEdgeData, const Polytope& polytope);

protected:
  std::weak_ptr<Transform> transform_ptr;
};


#endif //PLATFORMER_COLLIDER_H
