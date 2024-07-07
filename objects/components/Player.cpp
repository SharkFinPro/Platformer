#include "Player.h"
#include "RigidBody.h"
#include "Transform.h"
#include "../Object.h"
#include <SFML/Window/Keyboard.hpp>

Player::Player(PlayerControlType controlType)
  : Component{ComponentType::player}, speed{25}, jumpHeight{550}, controlType{controlType}, appliedForce{0}
{}

void Player::update([[maybe_unused]] const float dt)
{
  handleInput();
}

void Player::fixedUpdate([[maybe_unused]] const float dt)
{
  if (transform_ptr.expired())
  {
    transform_ptr = dynamic_pointer_cast<Transform>(owner->getComponent(ComponentType::transform));

    if (transform_ptr.expired())
    {
      return;
    }
  }

  if (std::shared_ptr<Transform> transform = transform_ptr.lock())
  {
    if (transform->getPosition().getY() > 2000)
    {
      transform->reset();
    }
  }

  if (rigidBody_ptr.expired())
  {
    rigidBody_ptr = dynamic_pointer_cast<RigidBody>(getOwner()->getComponent(ComponentType::rigidBody));

    if (rigidBody_ptr.expired())
    {
      return;
    }
  }

  if (std::shared_ptr<RigidBody> rigidBody = rigidBody_ptr.lock())
  {
    rigidBody->applyForce(appliedForce * dt);
  }

  appliedForce *= 0;
}

void Player::handleInput()
{
  if (rigidBody_ptr.expired())
  {
    rigidBody_ptr = dynamic_pointer_cast<RigidBody>(getOwner()->getComponent(ComponentType::rigidBody));

    if (rigidBody_ptr.expired())
    {
      return;
    }
  }

  float xForce = 0;
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
  {
    appliedForce.setX(xForce);
  }

  if (std::shared_ptr<RigidBody> rigidBody = rigidBody_ptr.lock())
  {
    if (!rigidBody->isFalling() && ((sf::Keyboard::isKeyPressed(sf::Keyboard::W) && controlType == PlayerControlType::WASD)
      || (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && controlType == PlayerControlType::ARROW)))
    {
      appliedForce.setY(-jumpHeight);
    }
  }
}