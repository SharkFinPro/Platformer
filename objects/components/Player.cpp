#include "Player.h"
#include "RigidBody.h"
#include "Transform.h"
#include "../GameObject.h"
#include <SFML/Window/Keyboard.hpp>

Player::Player(PlayerControlType controlType)
    : Component{ComponentType::player}, speed{10}, jumpHeight{14}, controlType{controlType}
{}

void Player::update(float dt)
{
    auto transform = dynamic_cast<Transform*>(owner->getComponent(ComponentType::transform));

    if (!transform)
        return;

    if (transform->getY() > 2000)
        transform->reset();
}

void Player::fixedUpdate(float dt)
{
    handleInput();
}

void Player::handleInput()
{
    auto rigidBody = dynamic_cast<RigidBody*>(getOwner()->getComponent(ComponentType::rigidBody));

    if (!rigidBody)
        return;

    if ((sf::Keyboard::isKeyPressed(sf::Keyboard::A) && controlType == PlayerControlType::WASD)
        || (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && controlType == PlayerControlType::ARROW))
    {
        rigidBody->setXvel(-speed);
    }
    if ((sf::Keyboard::isKeyPressed(sf::Keyboard::D) && controlType == PlayerControlType::WASD)
        || (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && controlType == PlayerControlType::ARROW))
    {
        rigidBody->setXvel(speed);
    }

    if (!rigidBody->isFalling() && ((sf::Keyboard::isKeyPressed(sf::Keyboard::W) && controlType == PlayerControlType::WASD)
        || (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && controlType == PlayerControlType::ARROW)))
    {
        rigidBody->setYvel(-jumpHeight);
    }
}