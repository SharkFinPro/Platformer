#include "Collider.h"
#include "../../GameObject.h"
#include "../Transform.h"
#include <cfloat>

Collider::Collider()
  : Component{ComponentType::collider}, transform{nullptr}
{}

bool Collider::collidesWith(GameObject* other, std::vector<Vec3<float>>& polytope)
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
  Vec3<float> direction = { transform->getPosition() - otherTransform->getPosition(), 0 };

  Vec3<float> support = getSupport(this, otherCollider, direction);

  simplex.addVertex(support);
  direction *= -1.0f;

  do
  {
    support = getSupport(this, otherCollider, direction);

    if (support.dot(direction) < 0)
      return false;

    simplex.addVertex(support);
  } while (!nextSimplex(simplex, direction));

  polytope.push_back(simplex.getA());
  polytope.push_back(simplex.getB());
  polytope.push_back(simplex.getC());

  return true;
}

Vec3<float> Collider::EPA(std::vector<Vec3<float>>& polytope, GameObject* other) {
  auto origin = Vec3{0.0f, 0.0f, 0.0f};

  // Todo: throw errors
  if (!transform)
  {
    transform = dynamic_cast<Transform*>(owner->getComponent(ComponentType::transform));

    if (!transform)
      return origin;
  }

  auto otherTransform = dynamic_cast<Transform*>(other->getComponent(ComponentType::transform));
  auto otherCollider = dynamic_cast<Collider*>(other->getComponent(ComponentType::collider));
  if (!otherTransform || !otherCollider)
    return origin;

  ///
  if (polytope.size() > 50)
    return origin;

  int closestA = 0;
  float minDist = FLT_MAX;

  for (int i = 0; i < polytope.size(); i++)
  {
    auto closest = getClosestPointOnLine(polytope[i], polytope[(i + 1) % polytope.size()], origin);

    float dist = (origin.getX() - closest.getX()) * (origin.getX() - closest.getX()) +
                 (origin.getY() - closest.getY()) * (origin.getY() - closest.getY());

    if (dist < minDist)
    {
      minDist = dist;
      closestA = i;
    }
  }

  // closest face
  auto a = polytope[closestA];
  auto b = polytope[(closestA + 1) % polytope.size()];

  //v
  auto v = getClosestPointOnLine(a, b, origin);

  //w
  auto w = getSupport(this, otherCollider, v);

  // project w -> v
  auto proj = v * (w.dot(v) / v.dot(v));

  //
  if (sqrtf(
      (proj.getX() - v.getX()) * (proj.getX() - v.getX()) +
      (proj.getY() - v.getY()) * (proj.getY() - v.getY())
  ) > 0.01f)
  {
    polytope.insert(polytope.begin() + closestA + 1, w);
    return EPA(polytope, other);
  }

  return v;
}

Vec3<float> Collider::getSupport(Collider* a, Collider* b, Vec3<float>& direction)
{
  return a->findFurthestPoint(direction) - b->findFurthestPoint(direction * -1.0f);
}

bool Collider::nextSimplex(Simplex& simplex, Vec3<float>& direction) {
  if (simplex.size() == 2)
    return line(simplex, direction);
  else if (simplex.size() == 3)
    return triangle(simplex, direction);
}

bool Collider::line(Simplex& simplex, Vec3<float>& direction)
{
  auto ab = simplex.getB() - simplex.getA();
  auto ao = simplex.getA() * -1.0f;

  direction = ab.cross(ao).cross(ab);

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

Vec3<float> Collider::getClosestPointOnLine(Vec3<float> a, Vec3<float> b, Vec3<float> c) {
  auto AB = b - a;

  auto dp = (c - a).dot(AB) / AB.dot(AB);

  if (dp < 0)
    dp = 0;
  else if (dp > 1)
    dp = 1;

  return a + (AB * dp);
}
