#include "Player.h"
#include "RigidBody.h"
#include "Transform.h"
#include "../GameObject.h"
#include <SFML/Window/Keyboard.hpp>

Player::Player()
    : Component{"Player"}, speed{10}, jumpHeight{14}
{}

void Player::update(float dt)
{
    Transform* transform = dynamic_cast<Transform*>(owner->getComponent("Transform"));

    if (!transform)
        return;

    if (transform->getY() > 2000)
        transform->setPosition(300, 400);
}

void Player::fixedUpdate(float dt)
{
    handleInput();
}

void Player::handleInput()
{
    RigidBody* rb = dynamic_cast<RigidBody*>(getOwner()->getComponent("RigidBody"));

    if (!rb)
        return;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        rb->setXvel(-speed);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        rb->setXvel(speed);

    if (!rb->isFalling() && (sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up)))
        rb->setYvel(-jumpHeight);
}