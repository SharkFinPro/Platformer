#include "Collider.h"
#include "../../GameObject.h"
#include "../Transform.h"

Collider::Collider()
  : Component{ComponentType::collider}, transform{nullptr}
{}

bool Collider::collidesWith(GameObject* other)
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

  return true;
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

  return false;
}

bool Collider::line(Simplex& simplex, Vec3<float>& direction)
{
  auto a = simplex.getA();
  auto b = simplex.getB();

  auto ab = b - a;
  auto ao = a * -1.0f;

  direction = ab.cross(ao).cross(ab);

  return false;
}

bool Collider::triangle(Simplex& simplex, Vec3<float>& direction)
{
  auto a = simplex.getA();
  auto b = simplex.getB();
  auto c = simplex.getC();

  auto ab = b - a;
  auto ac = c - a;
  auto ao = a * -1.0f;

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
