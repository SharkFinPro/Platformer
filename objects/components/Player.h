#ifndef PLATFORMER_PLAYER_H
#define PLATFORMER_PLAYER_H

#include "Component.h"
#include "../../math/Vec2.h"

class GameObject;
class Transform;
class RigidBody;

enum PlayerControlType { WASD, ARROW };

class Player : public Component
{
public:
  explicit Player(PlayerControlType controlType);

  void update(double dt) override;
  void fixedUpdate(double dt) override;

private:
  double speed;
  double jumpHeight;

  PlayerControlType controlType;

  Transform* transform;
  RigidBody* rigidBody;

  Vec2<double> appliedForce;

  void handleInput();
};


#endif //PLATFORMER_PLAYER_H
