#include "ObjectManager.h"
#include "Object.h"
#include "components/RigidBody.h"
#include "components/Transform.h"
#include "components/collisions/MeshCollider.h"

ObjectManager::ObjectManager()
  : window{nullptr}, fixedUpdateDt{1.0f / 50.0f}, timeAccumulator{0.0f}, ticks{0}
{}

ObjectManager::~ObjectManager()
{
  for (auto& object : objects)
    delete object;
}

void ObjectManager::update(float dt)
{
  fixedUpdate(dt);
  variableUpdate(dt);
}

void ObjectManager::addObject(Object* object)
{
  object->setOwner(this);
  objects.push_back(object);
}

void ObjectManager::removeObject(Object* object)
{
  for (int i = 0; i < static_cast<int>(objects.size()); i++)
    if (objects[i] == object)
    {
      auto tempObject = object;
      objects.erase(objects.begin() + i);
      delete tempObject;
      return;
    }
}

void ObjectManager::setWindow(sf::RenderWindow* window)
{
  this->window = window;
}

sf::RenderWindow* ObjectManager::getWindow() const
{
  return window;
}

void ObjectManager::variableUpdate(float dt)
{
  for (auto& object : objects)
    object->update(dt);
}

void ObjectManager::fixedUpdate(float dt)
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
    auto collider = dynamic_cast<Collider*>(object1->getComponent(ComponentType::collider));
    if (!collider)
      continue;

    std::vector<std::pair<Object*, std::vector<Vec3<float>>>> collisions;
    for (auto& object2 : objects)
    {
      if (object1 == object2)
        continue;

      if (!object2->getComponent(ComponentType::collider))
        continue;

      if (!object2->getComponent(ComponentType::transform))
        continue;

      std::vector<Vec3<float>> polytope;
      if (!collider->collidesWith(object2, polytope))
        continue;

      collisions.emplace_back(object2, polytope);
    }

    if (!collisions.empty())
    {
      auto rb = dynamic_cast<RigidBody*>(object1->getComponent(ComponentType::rigidBody));
      if (rb)
        rb->handleCollision(collider->minimumTranslationVector(collisions));
    }
  }
}
