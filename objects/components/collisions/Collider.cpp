#include "Collider.h"
#include "../../Object.h"
#include "../Transform.h"
#include <cfloat>
#include <cmath>
#include <iostream>
#include <optional>

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

  Vec3<float> support = getSupport(otherCollider, direction);
  simplex.addVertex(support);

  direction *= -1.0f;

  do
  {
    support = getSupport(otherCollider, direction);

    if (support.dot(direction) < 0)
      return false;

    simplex.addVertex(support);
  } while (!expandSimplex(simplex, direction));

  if (mtv != nullptr)
  {
    std::vector<Vec3<float>> polytope{simplex.getA(), simplex.getB(), simplex.getC()};

    *mtv = minimumTranslationVector(other, polytope);

//    polytope.push_back(simplex.getA());
//    polytope.push_back(simplex.getB());
//    polytope.push_back(simplex.getC());
  }

  return true;
}

Vec3<float> Collider::minimumTranslationVector(std::shared_ptr<Object> other, std::vector<Vec3<float>>& polytope)
{
  return -EPA(polytope, other);
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

struct ClosestEdgeData {
  Vec3<float> closestPoint;
  int closestIndex;
  Vec3<float> a;
  Vec3<float> b;
};

float findClosestEdge(std::vector<Vec3<float>>& polytope, ClosestEdgeData& closestEdgeData)
{
  Vec3<float> origin{0.0f, 0.0f, 0.0f};
  float minDist = FLT_MAX;
  int polytopeLength = polytope.size();

  for (int i = 0; i < polytopeLength; i++)
  {
    Vec3<float> current = polytope.at(i);
    Vec3<float> next = polytope.at((i + 1) % polytopeLength);
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

bool closeEnough(float minDistance, std::optional<float> previousMinDistance, Vec3<float> currentClosestPoint, std::optional<Vec3<float>> previousClosestPoint)
{
  if (!previousClosestPoint.has_value())
  {
    return false;
  }

  if (std::fabs(minDistance - previousMinDistance.value()) > 0.01)
  {
    return false;
  }

  float deltaX = std::fabs(currentClosestPoint.getX() - previousClosestPoint->getX());
  float deltaY = std::fabs(currentClosestPoint.getY() - previousClosestPoint->getY());

  return (deltaX + deltaY) < 1;
}

Vec3<float> getSearchDirection(ClosestEdgeData& closestEdgeData)
{
  Vec3<float> searchDirection = closestEdgeData.closestPoint;

  if (searchDirection.dot(searchDirection) < 0.01)
  {
    Vec3<float> AB = closestEdgeData.b - closestEdgeData.a;
    searchDirection = AB.cross({0, 0, AB.getX() < 0 ? 1.0f : -1.0f});
  }

  return searchDirection;
}

Vec3<float> Collider::EPA(std::vector<Vec3<float>>& polytope, const std::shared_ptr<Object>& other)
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

    Vec3<float> searchDirection = getSearchDirection(closestEdgeData);

    Vec3<float> supportPoint = getSupport(otherCollider, searchDirection);
    polytope.insert(polytope.begin() + closestEdgeData.closestIndex, supportPoint);

  }

//  if (fabs(closestPoint.getX()) < threshold && closestPoint.getX() != 0)
//    closestPoint.setX(0);
//
//  if (fabs(closestPoint.getY()) < threshold && closestPoint.getY() != 0)
//    closestPoint.setY(0);
//
  return closestEdgeData.closestPoint;
}
