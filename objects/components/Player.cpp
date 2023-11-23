#include "Player.h"
#include "RigidBody.h"
#include "Transform.h"
#include "../GameObject.h"
#include <SFML/Window/Keyboard.hpp>

Player::Player(PlayerControlType controlType)
  : Component{ComponentType::player}, speed{5}, jumpHeight{15}, controlType{controlType}, transform{nullptr}, rigidBody{nullptr}, appliedForce{0, 0}
{}

void Player::update([[maybe_unused]] double dt)
{
  handleInput();
}

void Player::fixedUpdate([[maybe_unused]] double dt)
{
  if (!transform)
    transform = dynamic_cast<Transform*>(owner->getComponent(ComponentType::transform));

  if (!transform)
    return;

  if (transform->getY() > 2000)
    transform->reset();

  if (!rigidBody)
    rigidBody = dynamic_cast<RigidBody*>(getOwner()->getComponent(ComponentType::rigidBody));

  if (!rigidBody)
    return;

  rigidBody->applyForce(appliedForce);
  appliedForce.setX(0);
  appliedForce.setY(0);
}

void Player::handleInput()
{
  if (!rigidBody)
    rigidBody = dynamic_cast<RigidBody*>(getOwner()->getComponent(ComponentType::rigidBody));

  if (!rigidBody)
    return;

  double xForce = 0;
  if ((sf::Keyboard::isKeyPressed(sf::Keyboard::A) && controlType == PlayerControlType::WASD)
    || (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && controlType == PlayerControlType::ARROW))
  {
    xForce -= speed;
  }
  if ((sf::Keyboard::isKeyPressed(sf::Keyboard::D) && controlType == PlayerControlType::WASD)
    || (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && controlType == PlayerControlType::ARROW))
  {
    xForce += speed;
  }

  if (xForce != 0)
    appliedForce.setX(xForce);

  if (!rigidBody->isFalling() && ((sf::Keyboard::isKeyPressed(sf::Keyboard::W) && controlType == PlayerControlType::WASD)
    || (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && controlType == PlayerControlType::ARROW)))
  {
    appliedForce.setY(-jumpHeight);
  }
}