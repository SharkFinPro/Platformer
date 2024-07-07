#include "ObjectManager.h"
#include "Object.h"
#include "components/RigidBody.h"
#include "components/Transform.h"
#include "components/collisions/MeshCollider.h"
#include <algorithm>

ObjectManager::ObjectManager()
  : window{nullptr}, fixedUpdateDt{1.0f / 50.0f}, timeAccumulator{0.0f}, ticks{0}
{}

void ObjectManager::update(const float dt)
{
  fixedUpdate(dt);
  variableUpdate(dt);
}

void ObjectManager::addObject(std::shared_ptr<Object> object)
{
  object->setOwner(this);
  objects.push_back(std::move(object));
}

[[maybe_unused]] bool ObjectManager::removeObject(const std::shared_ptr<Object>& object)
{
  for (size_t i = 0; i < objects.size(); i++)
  {
    if (objects[i] == object)
    {
      objects.erase(objects.begin() + static_cast<int>(i));
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

void ObjectManager::variableUpdate(const float dt)
{
  for (auto& object : objects)
  {
    object->update(dt);
  }
}

void ObjectManager::fixedUpdate(const float dt)
{
  timeAccumulator += dt;

  while (timeAccumulator >= fixedUpdateDt)
  {
    for (auto& object : objects)
    {
      object->fixedUpdate(fixedUpdateDt);
    }

    checkCollisions();

    timeAccumulator -= fixedUpdateDt;
    ticks++;
  }
}

void ObjectManager::checkCollisions()
{
  for (const auto& object : objects)
  {
    auto rigidBody = std::dynamic_pointer_cast<RigidBody>(object->getComponent(ComponentType::rigidBody));
    auto collider = std::dynamic_pointer_cast<Collider>(object->getComponent(ComponentType::collider));

    if (!rigidBody || !collider)
    {
      continue;
    }

    std::vector<std::shared_ptr<Object>> collidedObjects;
    findCollisions(object, collider, collidedObjects);

    if (!collidedObjects.empty())
    {
      handleCollisions(rigidBody, collider, collidedObjects);
    }
  }
}

void ObjectManager::findCollisions(const std::shared_ptr<Object>& object,
                                   const std::shared_ptr<Collider>& collider,
                                   std::vector<std::shared_ptr<Object>>& collidedObjects)
{
  for (const auto& other : objects)
  {
    if (other == object)
    {
      continue;
    }

    if (collider->collidesWith(other, nullptr))
    {
      collidedObjects.emplace_back(other);
    }
  }
}

void ObjectManager::handleCollisions(const std::shared_ptr<RigidBody>& rigidBody,
                                     const std::shared_ptr<Collider>& collider,
                                     std::vector<std::shared_ptr<Object>>& collidedObjects)
{
  std::vector<bool> chosenFlags(collidedObjects.size(), false);
  std::vector<float> distances;

  for (const auto& collidedObject : collidedObjects)
  {
    Vec3<float> mtv;
    collider->collidesWith(collidedObject, &mtv);

    distances.push_back(mtv.dot(mtv));
  }

  std::vector<float> sortedDistances = distances;
  std::sort(sortedDistances.begin(), sortedDistances.end(), std::greater<>());

  for (float sortedDistance : sortedDistances)
  {
    if (sortedDistance == 0)
    {
      break;
    }

    for (size_t j = 0; j < distances.size(); j++)
    {
      if (sortedDistance == distances[j] && !chosenFlags[j])
      {
        chosenFlags[j] = true;

        Vec3<float> mtv;
        if (collider->collidesWith(collidedObjects[j], &mtv))
        {
          rigidBody->handleCollision(mtv, collidedObjects[j]);
        }
      }
    }
  }
}
