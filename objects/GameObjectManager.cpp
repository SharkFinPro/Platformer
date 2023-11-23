#include "GameObjectManager.h"
#include "GameObject.h"
#include "components/BoxCollider.h"
#include "components/RigidBody.h"
#include "components/Transform.h"

GameObjectManager::GameObjectManager()
  : window{nullptr}, fixedUpdateDt{1.0f / 6.0f}, timeAccumulator{0.0f}, ticks{0}
{}

GameObjectManager::~GameObjectManager()
{
  for (auto& object : objects)
    delete object;
}

void GameObjectManager::update(float dt)
{
  fixedUpdate(dt);
  variableUpdate(dt);
}

void GameObjectManager::addObject(GameObject* object)
{
  object->setOwner(this);
  objects.push_back(object);
}

void GameObjectManager::removeObject(GameObject* object)
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

void GameObjectManager::setWindow(sf::RenderWindow* window)
{
  this->window = window;
}

sf::RenderWindow* GameObjectManager::getWindow() const
{
  return window;
}


void GameObjectManager::variableUpdate(float dt)
{
  for (auto& object : objects)
    object->update(dt);
}

void GameObjectManager::fixedUpdate(float dt)
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

void GameObjectManager::checkCollisions()
{
  for (auto& object1 : objects)
  {
    auto collider = dynamic_cast<BoxCollider*>(object1->getComponent(ComponentType::boxCollider));
    if (!collider)
      continue;

    auto rb = dynamic_cast<RigidBody*>(object1->getComponent(ComponentType::rigidBody));
    if (!rb)
      continue;

    std::vector<GameObject*> collisions;
    for (auto& object2 : objects)
    {
      if (object1 == object2)
        continue;

      if (!dynamic_cast<BoxCollider*>(object2->getComponent(ComponentType::boxCollider)))
        continue;

      auto otherTransform = dynamic_cast<Transform*>(object2->getComponent(ComponentType::transform));
      if (!otherTransform)
        continue;

      if (!collider->collidesWith(BoxCollider::getBoundingRectangle(otherTransform->getMesh())))
        continue;

      collisions.push_back(object2);
    }

    if (!collisions.empty())
    {
      auto penetrationVector = collider->getPenetrationVector(collisions);

      rb->handleCollision(penetrationVector);
    }
  }
}
