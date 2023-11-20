#include "Player.h"
#include "RigidBody.h"
#include "Transform.h"
#include "../GameObject.h"
#include <SFML/Window/Keyboard.hpp>

Player::Player(PlayerControlType controlType)
  : Component{ComponentType::player}, speed{5}, jumpHeight{15}, controlType{controlType}, transform{nullptr}, rigidBody{nullptr}
{}

void Player::update(float dt)
{
  handleInput();
}

void Player::fixedUpdate(float dt)
{
  if (!transform)
    transform = dynamic_cast<Transform*>(owner->getComponent(ComponentType::transform));

  if (!transform)
    return;

  if (transform->getY() > 2000)
    transform->reset();
}

void Player::handleInput()
{
  if (!rigidBody)
    rigidBody = dynamic_cast<RigidBody*>(getOwner()->getComponent(ComponentType::rigidBody));

  if (!rigidBody)
    return;

  if ((sf::Keyboard::isKeyPressed(sf::Keyboard::A) && controlType == PlayerControlType::WASD)
    || (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && controlType == PlayerControlType::ARROW))
  {
    rigidBody->applyForce(-speed, 0);
  }
  if ((sf::Keyboard::isKeyPressed(sf::Keyboard::D) && controlType == PlayerControlType::WASD)
    || (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && controlType == PlayerControlType::ARROW))
  {
    rigidBody->applyForce(speed, 0);
  }

  if (!rigidBody->isFalling() && ((sf::Keyboard::isKeyPressed(sf::Keyboard::W) && controlType == PlayerControlType::WASD)
    || (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && controlType == PlayerControlType::ARROW)))
  {
    rigidBody->applyForce(0, -jumpHeight);
  }
}