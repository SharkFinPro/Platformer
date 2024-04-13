#include "Collider.h"
#include "../../Object.h"
#include "../Transform.h"
#include <cfloat>
#include <iostream>

Collider::Collider()
  : Component{ComponentType::collider}
{}

bool Collider::collidesWith(const std::shared_ptr<Object>& other, std::vector<Vec3<float>>& polytope, Vec3<float> translation)
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
  Vec3<float> direction;

  if (std::shared_ptr<Transform> transform = transform_ptr.lock())
  {
    direction = Vec3<float>{ (transform->getPosition() - otherTransform->getPosition()).xy(), 0 } + translation;
  }
  Vec3<float> support = getSupport(otherCollider, direction, translation);

  simplex.addVertex(support);
  direction *= -1.0f;

  do
  {
    support = getSupport(otherCollider, direction, translation);

    if (support.dot(direction) < 0)
      return false;

    simplex.addVertex(support);
  } while (!nextSimplex(simplex, direction));

  polytope.push_back(simplex.getA());
  polytope.push_back(simplex.getB());
  if (simplex.size() == 3)
    polytope.push_back(simplex.getC());

  return true;
}

Vec3<float> Collider::minimumTranslationVector(std::vector<std::pair<std::shared_ptr<Object>, std::vector<Vec3<float>>>>& collisions)
{
  if (collisions.size() == 1)
    return EPA(collisions.at(0).second, collisions.at(0).first, Vec3<float>(0)) * -1.0f;

  auto finalMinimumTranslationVector = Vec3<float>(0);
  float xCollisions = 0;
  float yCollisions = 0;

  for (auto& collision : collisions)
  {
    auto minimumTranslationVector = EPA(collision.second, collision.first, Vec3<float>(0));

    finalMinimumTranslationVector += minimumTranslationVector;

    if (minimumTranslationVector.getX() != 0)
      xCollisions++;

    if (minimumTranslationVector.getY() != 0)
      yCollisions++;
  }

  if (finalMinimumTranslationVector.getX() != 0 && finalMinimumTranslationVector.getY() != 0)
  {
    if (std::fabs(finalMinimumTranslationVector.getX()) > std::fabs(finalMinimumTranslationVector.getY()))
    {
      auto theoreticalTransform = Vec3<float>{ -finalMinimumTranslationVector.getX(), 0.0f, 0.0f };

      finalMinimumTranslationVector.setY(0);

      yCollisions = 0;
      for (auto& collision : collisions)
      {
        std::vector<Vec3<float>> polytope;
        collidesWith(collision.first, polytope, theoreticalTransform);

        if (polytope.empty())
          continue;

        auto minimumTranslationVector = EPA(polytope, collision.first, theoreticalTransform);

        if (minimumTranslationVector.getY() != 0)
        {
          yCollisions++;
          finalMinimumTranslationVector.setY(finalMinimumTranslationVector.getY() + minimumTranslationVector.getY());
        }
      }
    }
    else
    {
      auto theoreticalTransform = Vec3<float>{ 0.0f, -finalMinimumTranslationVector.getY(), 0.0f };

      finalMinimumTranslationVector.setX(0);

      xCollisions = 0;
      for (auto& collision : collisions) {
        std::vector<Vec3<float>> polytope;
        collidesWith(collision.first, polytope, theoreticalTransform);

        if (polytope.empty())
          continue;

        auto minimumTranslationVector = EPA(polytope, collision.first, theoreticalTransform);

        if (minimumTranslationVector.getX() != 0) {
          xCollisions++;
          finalMinimumTranslationVector.setX(finalMinimumTranslationVector.getX() + minimumTranslationVector.getX());
        }
      }
    }
  }

  if (xCollisions != 0)
    finalMinimumTranslationVector.setX(finalMinimumTranslationVector.getX() / xCollisions);

  if (yCollisions != 0)
    finalMinimumTranslationVector.setY(finalMinimumTranslationVector.getY() / yCollisions);

  return finalMinimumTranslationVector * -1.0f;
}

Vec3<float> Collider::getSupport(const std::shared_ptr<Collider>& b, Vec3<float> direction, Vec3<float> translation)
{
  return findFurthestPoint(direction, translation) - b->findFurthestPoint(direction * -1.0f, Vec3<float>(0));
}

bool Collider::nextSimplex(Simplex& simplex, Vec3<float>& direction) {
  if (simplex.size() == 2)
    return line(simplex, direction);
  else if (simplex.size() == 3)
    return triangle(simplex, direction);

  return false;
}

bool Collider::line(Simplex& simplex, Vec3<float>& direction)
{
  auto ab = simplex.getB() - simplex.getA();
  auto ao = simplex.getA() * -1.0f;

  direction = ab.cross(ao).cross(ab);

  return direction.dot(ao) == 0;
}

bool Collider::triangle(Simplex& simplex, Vec3<float>& direction)
{
  auto ab = simplex.getB() - simplex.getA();
  auto ac = simplex.getC() - simplex.getA();
  auto ao = simplex.getA() * -1.0f;

  auto ABperp = ac.cross(ab).cross(ab);
  auto ACperp = ab.cross(ac).cross(ac);

  if (ABperp.dot(ao) > 0)
  {
    simplex.removeC();
    direction = ABperp;
    return false;
  }

  if (ACperp.dot(ao) > 0)
  {
    simplex.removeB();
    direction = ACperp;
    return false;
  }

  return true;
}

Vec3<float> Collider::getClosestPointOnLine(Vec3<float> a, Vec3<float> b, Vec3<float> c)
{
  auto AB = b - a;

  auto dp = (c - a).dot(AB) / AB.dot(AB);

  if (dp < 0)
    dp = 0;
  else if (dp > 1)
    dp = 1;

  return a + (AB * dp);
}

Vec3<float> Collider::EPA(std::vector<Vec3<float>>& polytope, const std::shared_ptr<Object>& other, Vec3<float> translation)
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

  float threshold = 0.01f;

  Vec3<float> closestPoint, a, b;
  unsigned int closestA = 0;

  Vec3<float> testPoint;
  Vec3<float> searchDirection;

  do
  {
    // Find the closest point on polytope
    float minDist = FLT_MAX;
    for (size_t i = 0; i < polytope.size(); i++)
    {
      Vec3<float> closest = getClosestPointOnLine(polytope.at(i), polytope.at((i + 1) % polytope.size()), {0.0f, 0.0f, 0.0f});

      float dist = (
        closest.getX() * closest.getX() +
        closest.getY() * closest.getY() +
        closest.getZ() * closest.getZ()
      );

      if (dist < minDist) {
        minDist = dist;
        closestPoint = closest;
        closestA = static_cast<unsigned int>(i);
        a = polytope.at(i);
        b = polytope.at((i + 1) % polytope.size());
      }
    }

    // Find search direction
    searchDirection = closestPoint;

    if ((a.getX() == 0 && b.getX() == 0) || (a.getY() == 0 && b.getY() == 0) || minDist == 0)
    {
      auto AB = b - a;

      searchDirection = {AB.getY(), AB.getX() * -1.0f, 0};

      for (auto j : polytope)
      {
        if (searchDirection.dot(j) > 0 && !(
            (j.getX() == a.getX() && j.getY() == a.getY()) ||
            (j.getX() == b.getX() && j.getY() == b.getY())))
        {
          searchDirection *= -1;
          break;
        }
      }
    }

    // Find and insert new point
    testPoint = getSupport(otherCollider, searchDirection.normalized(), translation);

    if (testPoint.dot(searchDirection) > 0
      && !(a.getX() == testPoint.getX() && a.getY() == testPoint.getY())
      && !(b.getX() == testPoint.getX() && b.getY() == testPoint.getY()))
    {
      polytope.insert(polytope.begin() + closestA + 1, testPoint);

      if (polytope.size() == 3)
      {
        searchDirection *= -1;
        testPoint = getSupport(otherCollider, searchDirection.normalized(), translation);

        if (testPoint.dot(searchDirection) > 0
            && !(a.getX() == testPoint.getX() && a.getY() == testPoint.getY())
            && !(b.getX() == testPoint.getX() && b.getY() == testPoint.getY()))
        {
          polytope.insert(polytope.begin() + closestA, testPoint);
        }
      }
    }
    else
    {
      break;
    }
  } while (polytope.size() < 25);

  if (fabs(closestPoint.getX()) < threshold && closestPoint.getX() != 0)
    closestPoint.setX(0);

  if (fabs(closestPoint.getY()) < threshold && closestPoint.getY() != 0)
    closestPoint.setY(0);

  return closestPoint;
}
