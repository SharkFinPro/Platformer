#include "Collider.h"
#include "../../GameObject.h"
#include "../Transform.h"
#include <cfloat>
#include <iostream>

Collider::Collider()
  : Component{ComponentType::collider}, transform{nullptr}
{}

bool Collider::collidesWith(GameObject* other, std::vector<Vec3<float>>& polytope, Vec3<float> translation)
{
  if (!transform)
  {
    transform = dynamic_cast<Transform*>(owner->getComponent(ComponentType::transform));

    if (!transform)
      return false;
  }

  auto otherTransform = dynamic_cast<Transform*>(other->getComponent(ComponentType::transform));
  auto otherCollider = dynamic_cast<Collider*>(other->getComponent(ComponentType::collider));
  if (!otherTransform || !otherCollider)
    return false;

  Simplex simplex;
  auto direction = Vec3<float>{ transform->getPosition() - otherTransform->getPosition(), 0 } + translation;

  Vec3<float> support = getSupport(this, otherCollider, direction, translation);

  simplex.addVertex(support);
  direction *= -1.0f;

  do
  {
    support = getSupport(this, otherCollider, direction, translation);

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

Vec2<float> Collider::getPenetrationVector(std::vector<std::pair<GameObject*, std::vector<Vec3<float>>>>& collisions)
{
  Vec2<float> finalPenetrationVector = {0, 0};
  float xCollisions = 0;
  float yCollisions = 0;

  for (auto& collision : collisions)
  {
    auto penetrationVector = EPA(collision.second, collision.first, {0, 0, 0});

    finalPenetrationVector += penetrationVector.xy();

    if (penetrationVector.getX() != 0)
      xCollisions++;

    if (penetrationVector.getY() != 0)
      yCollisions++;
  }

  if (finalPenetrationVector.getX() != 0 && finalPenetrationVector.getY() != 0)
  {
    if (std::fabs(finalPenetrationVector.getX()) > std::fabs(finalPenetrationVector.getY()))
    {
      auto theoreticalTransform = Vec3<float>{ -finalPenetrationVector.getX(), 0.0f, 0.0f };

      finalPenetrationVector.setY(0);

      yCollisions = 0;
      for (auto& collision : collisions)
      {
        std::vector<Vec3<float>> polytope;
        collidesWith(collision.first, polytope, theoreticalTransform);

        if (polytope.empty())
          continue;

        auto penetrationVector = EPA(polytope, collision.first, theoreticalTransform);

        if (penetrationVector.getY() != 0)
        {
          yCollisions++;
          finalPenetrationVector.setY(finalPenetrationVector.getY() + penetrationVector.getY());
        }
      }
    }
    else
    {
      auto theoreticalTransform = Vec3<float>{ 0.0f, -finalPenetrationVector.getY(), 0.0f };

      finalPenetrationVector.setX(0);

      xCollisions = 0;
      for (auto& collision : collisions) {
        std::vector<Vec3<float>> polytope;
        collidesWith(collision.first, polytope, theoreticalTransform);

        if (polytope.empty())
          continue;

        auto penetrationVector = EPA(polytope, collision.first, theoreticalTransform);

        if (penetrationVector.getX() != 0) {
          xCollisions++;
          finalPenetrationVector.setX(finalPenetrationVector.getX() + penetrationVector.getX());
        }
      }
    }
  }

  if (xCollisions != 0)
    finalPenetrationVector.setX(finalPenetrationVector.getX() / xCollisions);

  if (yCollisions != 0)
    finalPenetrationVector.setY(finalPenetrationVector.getY() / yCollisions);

  return finalPenetrationVector;
}

Vec3<float> Collider::getSupport(Collider* a, Collider* b, Vec3<float> direction, Vec3<float> translation)
{
  return a->findFurthestPoint(direction, translation) - b->findFurthestPoint(direction * -1.0f, {0, 0, 0});
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

  if (direction.dot(ao) == 0)
    return true;

  return false;
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

Vec3<float> Collider::EPA(std::vector<Vec3<float>>& polytope, GameObject* other, Vec3<float> translation)
{
  if (!transform)
  {
    transform = dynamic_cast<Transform*>(owner->getComponent(ComponentType::transform));

    if (!transform)
      throw std::runtime_error("Collider::EPA::Missing Transform");
  }

  auto otherTransform = dynamic_cast<Transform*>(other->getComponent(ComponentType::transform));
  auto otherCollider = dynamic_cast<Collider*>(other->getComponent(ComponentType::collider));
  if (!otherTransform || !otherCollider)
    throw std::runtime_error("Collider::EPA::Missing Transform/Collider");

  float threshold = 0.0001f;

  Vec3<float> closestPoint, a, b;
  int closestA;

  Vec3<float> testPoint;
  Vec3<float> searchDirection;

  // Find closest point on polytope
  float minDist = FLT_MAX;
  for (int i = 0; i < polytope.size(); i++)
  {
    Vec3<float> closest = getClosestPointOnLine(polytope.at(i), polytope.at((i + 1) % polytope.size()), {0.0f, 0.0f, 0.0f});

    float dist =
      closest.getX() * closest.getX() +
      closest.getY() * closest.getY() +
      closest.getZ() * closest.getZ();

    if (dist < minDist) {
      minDist = dist;
      closestPoint = closest;
      closestA = i;
      a = polytope.at(i);
      b = polytope.at((i + 1) % polytope.size());
    }
  }

  if (polytope.size() < 25)
  {
    // Find search direction
    searchDirection = closestPoint * 100000.0f;

    if ((a.getX() == 0 && b.getX() == 0) || (a.getY() == 0 && b.getY() == 0))
    {
      auto AB = b - a;

      searchDirection = {AB.getY(), AB.getX() * -1.0f, 0};

      if (searchDirection.dot(a * -1) > 0)
      {
        searchDirection *= -1;
      }
      else
      {
        for (auto j : polytope)
        {
          if (searchDirection.dot(j) > 0 && !(
              (j.getX() == a.getX() && j.getY() == a.getY()) ||
              (j.getX() == b.getX() && j.getY() == b.getY())
          ))
          {
            searchDirection = searchDirection * -1;
            break;
          }
        }
      }
    }

    // Find and insert new point
    testPoint = getSupport(this, otherCollider, searchDirection.normalized(), translation);

    if (testPoint.dot(searchDirection) > 0)
    {
      bool dupe = false;
      for (auto v : polytope) {
        if (v.getX() == testPoint.getX() && v.getY() == testPoint.getY())
        {
          dupe = true;
          break;
        }
      }

      if (!dupe)
      {
        polytope.insert(polytope.begin() + closestA + 1, testPoint);

        if (polytope.size() == 3)
        {
          searchDirection *= -1;
          testPoint = getSupport(this, otherCollider, searchDirection.normalized(), translation);

          if (testPoint.dot(searchDirection) > 0)
          {
            for (auto v : polytope) {
              if (v.getX() == testPoint.getX() && v.getY() == testPoint.getY())
              {
                dupe = true;
                break;
              }
            }

            if (!dupe)
            {
              polytope.insert(polytope.begin() + closestA, testPoint);
            }
          }
        }

        return EPA(polytope, other, translation);
      }
    }
  }

  if (fabs(closestPoint.getX()) < threshold && closestPoint.getX() != 0)
    closestPoint.setX(0);

  if (fabs(closestPoint.getY()) < threshold && closestPoint.getY() != 0)
    closestPoint.setY(0);

  return closestPoint;
}
