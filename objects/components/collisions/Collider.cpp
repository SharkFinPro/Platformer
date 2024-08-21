#include "Collider.h"
#include "../../Object.h"
#include "../Transform.h"
#include <cfloat>
#include <cmath>

Collider::Collider()
  : Component{ComponentType::collider}
{}

bool Collider::collidesWith(const std::shared_ptr<Object>& other, Vec3<float>* mtv)
{
  if (transform_ptr.expired())
  {
    transform_ptr = dynamic_pointer_cast<Transform>(owner->getComponent(ComponentType::transform));

    if (transform_ptr.expired())
    {
      throw std::runtime_error("Collider::EPA::Missing Transform");
    }
  }

  const auto otherTransform = dynamic_pointer_cast<Transform>(other->getComponent(ComponentType::transform));
  const auto otherCollider = dynamic_pointer_cast<Collider>(other->getComponent(ComponentType::collider));
  if (!otherTransform || !otherCollider)
  {
    return false;
  }

  Simplex simplex;
  Vec3<float> direction{1, 0, 0};

  Vec3<float> support = getSupport(otherCollider, direction.normalized());
  simplex.addVertex(support);

  direction *= -1.0f;

  do
  {
    support = getSupport(otherCollider, direction.normalized());

    if (support.dot(direction) < 0)
    {
      return false;
    }

    simplex.addVertex(support);
  } while (!expandSimplex(simplex, direction));

  if (mtv != nullptr)
  {
    Polytope polytope{simplex.getA(), simplex.getB(), simplex.getC()};

    *mtv = -EPA(polytope, other);
  }

  return true;
}

Vec3<float> Collider::getSupport(const std::shared_ptr<Collider>& b, const Vec3<float>& direction)
{
  return findFurthestPoint(direction) - b->findFurthestPoint(-direction);
}

bool Collider::expandSimplex(Simplex& simplex, Vec3<float>& direction)
{
  switch (simplex.size())
  {
    case 2:
      return lineCase(simplex, direction);
    case 3:
      return triangleCase(simplex, direction);
    default:
      return false;
  }
}

bool Collider::lineCase(const Simplex& simplex, Vec3<float>& direction)
{
  const auto AB = simplex.getB() - simplex.getA();
  const auto AO = -simplex.getA();

  direction = AB.cross(AO).cross(AB);

  if (direction.dot(direction) == 0)
  {
    direction = AB.cross({0, 0, 1});
  }

  return false;
}

bool Collider::triangleCase(Simplex& simplex, Vec3<float>& direction)
{
  const auto AB = simplex.getB() - simplex.getA();
  const auto AC = simplex.getC() - simplex.getA();
  const auto AO = -simplex.getA();

  const auto ABperp = AC.cross(AB).cross(AB);
  const auto ACperp = AB.cross(AC).cross(AC);

  if (ABperp.dot(AO) > 0)
  {
    simplex.removeC();
    direction = ABperp;
    return false;
  }

  if (ACperp.dot(AO) > 0)
  {
    simplex.removeB();
    direction = ACperp;
    return false;
  }

  return true;
}

Vec3<float> Collider::closestPointOnLine(const Vec3<float>& a, const Vec3<float>& b)
{
  const auto AB = b - a;
  const auto AO = -a;

  const auto projection = AO.dot(AB) / AB.dot(AB);

  return a + AB * projection;
}

float Collider::findClosestEdge(const Polytope& polytope, ClosestEdgeData& closestEdgeData)
{
  float minDist = FLT_MAX;
  const int polytopeLength = static_cast<int>(polytope.size());

  for (int i = 0; i < polytopeLength; i++)
  {
    Vec3<float> current = polytope[i];
    Vec3<float> next = polytope[(i + 1) % polytopeLength];
    Vec3<float> c = closestPointOnLine(current, next);

    if (const float dist = c.dot(c); dist < minDist)
    {
      minDist = dist;
      closestEdgeData.closestPoint = c;
      closestEdgeData.closestIndex = i;
      closestEdgeData.a = current;
      closestEdgeData.b = next;
    }
  }

  return minDist;
}

bool Collider::closeEnough(const float minDistance, const std::optional<float>& previousMinDistance, const Vec3<float>& currentClosestPoint, const std::optional<Vec3<float>>& previousClosestPoint)
{
  if (!previousClosestPoint.has_value())
  {
    return false;
  }

  if (std::fabs(minDistance - previousMinDistance.value()) >= 0.01)
  {
    return false;
  }

  const float deltaX = std::fabs(currentClosestPoint.getX() - previousClosestPoint->getX());
  const float deltaY = std::fabs(currentClosestPoint.getY() - previousClosestPoint->getY());

  return deltaX + deltaY < 1.0f;
}

Vec3<float> Collider::getSearchDirection(const ClosestEdgeData& closestEdgeData, const Polytope& polytope)
{
  Vec3<float> searchDirection = closestEdgeData.closestPoint;

  if (searchDirection.dot(searchDirection) < 0.01)
  {
    const Vec3<float> AB = closestEdgeData.b - closestEdgeData.a;
    searchDirection = AB.cross({0, 0, AB.getX() < 0 ? 1.0f : -1.0f});

    for (size_t i = 0; i < polytope.size(); i++)
    {
      if (i == static_cast<size_t>(closestEdgeData.closestIndex) || i == (closestEdgeData.closestIndex + 1) % polytope.size())
      {
        continue;
      }

      if (searchDirection.dot(polytope[i]) > 0)
      {
        searchDirection *= -1;
        break;
      }
    }
  }

  return searchDirection;
}

Vec3<float> Collider::EPA(Polytope& polytope, const std::shared_ptr<Object>& other)
{
  if (transform_ptr.expired())
  {
    transform_ptr = dynamic_pointer_cast<Transform>(owner->getComponent(ComponentType::transform));

    if (transform_ptr.expired())
    {
      throw std::runtime_error("Collider::EPA::Missing Transform");
    }
  }

  const auto otherTransform = dynamic_pointer_cast<Transform>(other->getComponent(ComponentType::transform));
  const auto otherCollider = dynamic_pointer_cast<Collider>(other->getComponent(ComponentType::collider));
  if (!otherTransform || !otherCollider)
  {
    throw std::runtime_error("Collider::EPA::Missing Transform/Collider");
  }

  std::optional<Vec3<float>> previousClosestPoint;
  std::optional<float> previousMinDist;
  ClosestEdgeData closestEdgeData;

  constexpr int maxIterations = 25;
  int iterations = 0;
  while (iterations < maxIterations)
  {
    iterations++;
    float minDist = findClosestEdge(polytope, closestEdgeData);

    if (closeEnough(minDist, previousMinDist, closestEdgeData.closestPoint, previousClosestPoint))
    {
      break;
    }

    Vec3<float> searchDirection = getSearchDirection(closestEdgeData, polytope);

    Vec3<float> supportPoint = getSupport(otherCollider, searchDirection.normalized());
    polytope.insert(polytope.begin() + closestEdgeData.closestIndex + 1, supportPoint);

    previousMinDist = minDist;
    previousClosestPoint = closestEdgeData.closestPoint;
  }

  return closestEdgeData.closestPoint;
}
