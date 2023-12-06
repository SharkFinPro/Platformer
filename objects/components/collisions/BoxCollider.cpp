#include "BoxCollider.h"
#include "../Transform.h"
#include "../../GameObject.h"
#include <cmath>
#include <cfloat>

BoxCollider::BoxCollider()
  : Component{ComponentType::boxCollider}, transform{nullptr}
{}

bool BoxCollider::collidesWith(BoundingRectangle r2)
{
  if (!transform)
    transform = dynamic_cast<Transform*>(owner->getComponent(ComponentType::transform));

  if (!transform)
    return false;

  auto r1 = getBoundingRectangle(transform->getMesh());

  float mdX =  r1.left - r2.right;
  float mdY = r1.top - r2.bottom;
  float mdW = r1.width() + r2.width();
  float mdH = r1.height() + r2.height();

  return mdX <= 0 && mdX + mdW >= 0 && mdY <= 0 && mdY + mdH >= 0;
}

Vec2<float> BoxCollider::getPenetrationVector(const std::vector<GameObject *>& objects)
{
  Vec2<float> finalPenetrationVector = {0, 0};
  float xCollisions = 0;
  float yCollisions = 0;

  for (auto& object : objects)
  {
    auto penetrationVector = getPenetration(object);

    if (penetrationVector.getX() != 0)
    {
      xCollisions++;
      finalPenetrationVector.setX(finalPenetrationVector.getX() + penetrationVector.getX());
    }

    if (penetrationVector.getY() != 0)
    {
      yCollisions++;
      finalPenetrationVector.setY(finalPenetrationVector.getY() + penetrationVector.getY());
    }
  }

  if (finalPenetrationVector.getX() != 0 && finalPenetrationVector.getY() != 0)
  {
    if (std::fabs(finalPenetrationVector.getX()) > std::fabs(finalPenetrationVector.getY()))
    {
      auto theoreticalRectangle = getBoundingRectangle(transform->getMesh());
      theoreticalRectangle.left -= finalPenetrationVector.getX();
      theoreticalRectangle.right -= finalPenetrationVector.getX();

      finalPenetrationVector.setY(0);

      yCollisions = 0;
      for (auto& object : objects)
      {
        auto penetrationVector = getTheoreticalPenetration(theoreticalRectangle, object);

        if (penetrationVector.getY() != 0)
        {
          yCollisions++;
          finalPenetrationVector.setY(finalPenetrationVector.getY() + penetrationVector.getY());
        }
      }
    }
    else
    {
      auto theoreticalRectangle = getBoundingRectangle(transform->getMesh());
      theoreticalRectangle.top -= finalPenetrationVector.getY();
      theoreticalRectangle.bottom -= finalPenetrationVector.getY();

      finalPenetrationVector.setX(0);

      xCollisions = 0;
      for (auto& object : objects)
      {
        auto penetrationVector = getTheoreticalPenetration(theoreticalRectangle, object);

        if (penetrationVector.getX() != 0)
        {
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

Vec2<float> BoxCollider::getPenetration(GameObject* object)
{
  if (!transform)
    transform = dynamic_cast<Transform*>(owner->getComponent(ComponentType::transform));

  if (!transform)
    return { 0, 0 };

  auto otherTransform = dynamic_cast<Transform*>(object->getComponent(ComponentType::transform));

  if (!transform || !otherTransform)
    return { 0, 0 };

  auto r1 = getBoundingRectangle(transform->getMesh());
  auto r2 = getBoundingRectangle(otherTransform->getMesh());

  return getActualPenetration(r1, r2);
}

Vec2<float> BoxCollider::getTheoreticalPenetration(BoundingRectangle boundingRectangle, GameObject* object)
{
  auto otherTransform = dynamic_cast<Transform*>(object->getComponent(ComponentType::transform));

  if (!otherTransform)
    return { 0, 0 };

  auto r2 = getBoundingRectangle(otherTransform->getMesh());

  return getActualPenetration(boundingRectangle, r2);
}

Vec2<float> BoxCollider::getActualPenetration(BoundingRectangle r1, BoundingRectangle r2)
{
  float mdX =  r1.left - r2.right;
  float mdY = r1.top - r2.bottom;
  float mdW = r1.width() + r2.width();
  float mdH = r1.height() + r2.height();

  Vec2<float> min{mdX, mdY};
  Vec2<float> max{mdX + mdW, mdY + mdH};

  float minDist = std::fabs(min.getX());
  Vec2<float> penetrationVector{min.getX(), 0};

  if (std::fabs(max.getX()) < minDist)
  {
    minDist = std::fabs(max.getX());
    penetrationVector.setX(max.getX());
  }

  if (std::fabs(min.getY()) < minDist)
  {
    minDist = std::fabs(min.getY());
    penetrationVector.setX(0);
    penetrationVector.setY(min.getY());
  }

  if (std::fabs(max.getY()) < minDist)
  {
    penetrationVector.setX(0);
    penetrationVector.setY(max.getY());
  }

  return penetrationVector;
}

BoundingRectangle BoxCollider::getBoundingRectangle(const std::vector<Vec2<float>>& mesh)
{
  Vec2<float> x, y;

  float maxDot = -FLT_MAX;
  Vec2<float> direction = { -1, 0 };
  for (auto& m : mesh)
  {
    if (m.dot(direction) > maxDot)
    {
      maxDot = m.dot(direction);
      x.setX(m.getX());
    }
  }

  maxDot = -FLT_MAX;
  direction = { 1, 0 };
  for (auto& m : mesh)
  {
    if (m.dot(direction) > maxDot)
    {
      maxDot = m.dot(direction);
      x.setY(m.getX());
    }
  }

  maxDot = -FLT_MAX;
  direction = { 0, -1 };
  for (auto& m : mesh)
  {
    if (m.dot(direction) > maxDot)
    {
      maxDot = m.dot(direction);
      y.setX(m.getY());
    }
  }

  maxDot = -FLT_MAX;
  direction = { 0, 1 };
  for (auto& m : mesh)
  {
    if (m.dot(direction) > maxDot)
    {
      maxDot = m.dot(direction);
      y.setY(m.getY());
    }
  }

  return BoundingRectangle{x.getX(), x.getY(), y.getX(), y.getY()};
}