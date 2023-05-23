#include "GameObjectManager.h"
#include "GameObject.h"
#include "components/BoxCollider.h"
#include "components/RigidBody.h"
#include "components/Transform.h"

GameObjectManager::GameObjectManager()
    : window{nullptr}, fixedUpdateDt{1.0f / 60.0f}, timeAccumulator{0}
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
    for (int i = 0; i < objects.size(); i++)
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

sf::RenderWindow *GameObjectManager::getWindow() const
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

        checkCollisions(dt);

        timeAccumulator -= fixedUpdateDt;
    }
}

void GameObjectManager::checkCollisions(float dt)
{
    for (int i = 0; i < objects.size() - 1; i++)
    {
        auto collider = dynamic_cast<BoxCollider*>(objects[i]->getComponent(ComponentType::boxCollider));
        if (!collider)
            continue;

        auto rb = dynamic_cast<RigidBody*>(objects[i]->getComponent(ComponentType::rigidBody));

        for (int j = i + 1; j < objects.size(); j++)
        {
            auto otherCollider = dynamic_cast<BoxCollider*>(objects[j]->getComponent(ComponentType::boxCollider));
            if (!otherCollider)
                continue;

            auto otherTransform = dynamic_cast<Transform*>(objects[j]->getComponent(ComponentType::transform));
            if (!otherTransform)
                continue;

            if (!collider->collidesWith(otherTransform->getBoundingRectangle()))
                continue;

            auto rbOther = dynamic_cast<RigidBody*>(objects[j]->getComponent(ComponentType::rigidBody));

            if (rb)
                rb->handleCollision(objects[j], dt);

            if (rbOther)
                rbOther->handleCollision(objects[i], dt);
        }
    }
}
