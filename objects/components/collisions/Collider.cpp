#include "Collider.h"
#include "../../Object.h"
#include "../Transform.h"
#include <cfloat>
#include <cmath>
#include <iostream>

Collider::Collider()
  : Component{ComponentType::collider}
{}

bool Collider::collidesWith(const std::shared_ptr<Object>& other, Vec3<float>* mtv)
{
  if (transform_ptr.expired())
  {
    transform_ptr = dynamic_pointer_cast<Transform>(owner->getComponent(ComponentType::transform));

    if (transform_ptr.expired())
      throw std::runtime_error("Collider::EPA::Missing Transform");
  }

  auto otherTransform = dynamic_pointer_cast<Transform>(other->getComponent(ComponentType::transform));
  auto otherCollider = dynamic_pointer_cast<Collider>(other->getComponent(ComponentType::collider));
  if (!otherTransform || !otherCollider)
    return false;

  Simplex simplex;
  Vec3<float> direction{1, 0, 0};

  Vec3<float> support = getSupport(otherCollider, direction.normalized());
  simplex.addVertex(support);

  direction *= -1.0f;

  do
  {
    support = getSupport(otherCollider, direction.normalized());

    if (support.dot(direction) < 0)
      return false;

    simplex.addVertex(support);
  } while (!expandSimplex(simplex, direction));

  if (mtv != nullptr)
  {
    Polytope polytope{simplex.getA(), simplex.getB(), simplex.getC()};

    *mtv = -EPA(polytope, other);
  }

  return true;
}

Vec3<float> Collider::getSupport(const std::shared_ptr<Collider>& b, Vec3<float> direction)
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

bool Collider::lineCase(Simplex& simplex, Vec3<float>& direction)
{
  auto AB = simplex.getB() - simplex.getA();
  auto AO = -simplex.getA();

  direction = AB.cross(AO).cross(AB);

  if (direction.dot(direction) == 0)
  {
    direction = AB.cross({0, 0, 1});
  }

  return false;
}

bool Collider::triangleCase(Simplex& simplex, Vec3<float>& direction)
{
  auto AB = simplex.getB() - simplex.getA();
  auto AC = simplex.getC() - simplex.getA();
  auto AO = -simplex.getA();

  auto ABperp = AC.cross(AB).cross(AB);
  auto ACperp = AB.cross(AC).cross(AC);

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

Vec3<float> Collider::closestPointOnLine(Vec3<float> a, Vec3<float> b, Vec3<float> c)
{
  auto AB = b - a;
  auto AC = c - a;

  auto projection = AC.dot(AB) / AB.dot(AB);

  return a + (AB * projection);
}



float Collider::findClosestEdge(Polytope& polytope, ClosestEdgeData& closestEdgeData)
{
  Vec3<float> origin{0.0f, 0.0f, 0.0f};
  float minDist = FLT_MAX;
  size_t polytopeLength = polytope.size();

  for (int i = 0; i < polytopeLength; i++)
  {
    Vec3<float> current = polytope[i];
    Vec3<float> next = polytope[(i + 1) % polytopeLength];
    Vec3<float> c = Collider::closestPointOnLine(current, next, origin);
    float dist = c.dot(c);

    if (dist < minDist)
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

bool Collider::closeEnough(float minDistance, std::optional<float> previousMinDistance, Vec3<float> currentClosestPoint, std::optional<Vec3<float>> previousClosestPoint)
{
  if (!previousClosestPoint.has_value())
  {
    return false;
  }

  if (std::fabs(minDistance - previousMinDistance.value()) >= 0.01)
  {
    return false;
  }

  float deltaX = std::fabs(currentClosestPoint.getX() - previousClosestPoint->getX());
  float deltaY = std::fabs(currentClosestPoint.getY() - previousClosestPoint->getY());

  return (deltaX + deltaY) < 1;
}

Vec3<float> Collider::getSearchDirection(ClosestEdgeData& closestEdgeData, Polytope& polytope)
{
  Vec3<float> searchDirection = closestEdgeData.closestPoint;

  if (searchDirection.dot(searchDirection) < 0.01)
  {
    Vec3<float> AB = closestEdgeData.b - closestEdgeData.a;
    searchDirection = AB.cross({0, 0, AB.getX() < 0 ? 1.0f : -1.0f});

    for (int i = 0; i < polytope.size(); i++)
    {
      if (i == closestEdgeData.closestIndex || i == closestEdgeData.closestIndex + 1)
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
      throw std::runtime_error("Collider::EPA::Missing Transform");
  }

  auto otherTransform = dynamic_pointer_cast<Transform>(other->getComponent(ComponentType::transform));
  auto otherCollider = dynamic_pointer_cast<Collider>(other->getComponent(ComponentType::collider));
  if (!otherTransform || !otherCollider)
    throw std::runtime_error("Collider::EPA::Missing Transform/Collider");

  std::optional<Vec3<float>> previousClosestPoint;
  std::optional<float> previousMinDist;
  ClosestEdgeData closestEdgeData;

  int maxIterations = 25;
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
