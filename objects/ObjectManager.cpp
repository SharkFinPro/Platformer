#include "ObjectManager.h"
#include "Object.h"
#include "components/RigidBody.h"
#include "components/Transform.h"
#include "components/collisions/MeshCollider.h"
#include <algorithm>

ObjectManager::ObjectManager()
  : window{nullptr}, fixedUpdateDt{1.0f / 50.0f}, timeAccumulator{0.0f}, ticks{0}
{}

void ObjectManager::update(const float& dt)
{
  fixedUpdate(dt);
  variableUpdate(dt);
}

void ObjectManager::addObject(std::shared_ptr<Object> object)
{
  object->setOwner(this);
  objects.push_back(std::move(object));
}

bool ObjectManager::removeObject(const std::shared_ptr<Object>& object)
{
  for (int i = 0; i < static_cast<int>(objects.size()); i++)
  {
    if (objects[i] == object)
    {
      objects.erase(objects.begin() + i);
      return true;
    }
  }

  return false;
}

void ObjectManager::setWindow(sf::RenderWindow* window_)
{
  window = window_;
}

sf::RenderWindow* ObjectManager::getWindow() const
{
  return window;
}

void ObjectManager::variableUpdate(const float& dt)
{
  for (auto& object : objects)
    object->update(dt);
}

void ObjectManager::fixedUpdate(const float& dt)
{
  timeAccumulator += dt;

  while (timeAccumulator >= fixedUpdateDt)
  {
    for (auto& object : objects)
      object->fixedUpdate(fixedUpdateDt);

    checkCollisions();

    timeAccumulator -= fixedUpdateDt;
    ticks++;
  }
}

void ObjectManager::checkCollisions()
{
  for (auto& object1 : objects)
  {
    auto collider = dynamic_pointer_cast<Collider>(object1->getComponent(ComponentType::collider));
    if (!collider)
      continue;

    std::vector<std::shared_ptr<Object>> collidedObjects;
    for (auto& object2 : objects)
    {
      if (object1 == object2)
        continue;

      if (!object2->getComponent(ComponentType::collider))
        continue;

      if (!object2->getComponent(ComponentType::transform))
        continue;

      if (collider->collidesWith(object2, nullptr))
      {
        collidedObjects.emplace_back(object2);
      }
    }

    if (collidedObjects.empty())
    {
      continue;
    }

    auto rb = dynamic_pointer_cast<RigidBody>(object1->getComponent(ComponentType::rigidBody));
    if (!rb) {
      continue;
    }

    std::vector<float> distances;
    std::vector<bool> chosenFlags;
    for (size_t i = 0; i < collidedObjects.size(); i++)
    {
      Vec3<float> mtv;
      collider->collidesWith(collidedObjects[i], &mtv);

      distances.push_back(mtv.dot(mtv));
      chosenFlags.push_back(false);
    }

    std::vector<float> sortedDistances = distances;
    std::sort(sortedDistances.begin(), sortedDistances.end(), std::greater<>());

    for (size_t i = 0; i < sortedDistances.size(); i++)
    {
      if (sortedDistances[i] == 0)
      {
        break;
      }

      for (size_t j = 0; j < distances.size(); j++)
      {
        if (sortedDistances[i] == distances[j] && !chosenFlags[j])
        {
          chosenFlags[j] = true;

          Vec3<float> mtv;
          if (collider->collidesWith(collidedObjects[j], &mtv))
          {
            rb->handleCollision(mtv, collidedObjects[j]);
          }
          continue;
        }
      }
    }
  }
}
