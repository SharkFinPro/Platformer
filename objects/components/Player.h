#ifndef PLATFORMER_PLAYER_H
#define PLATFORMER_PLAYER_H

#include "Component.h"
#include "../../math/Vec3.h"
#include <memory>

class Object;
class Transform;
class RigidBody;

enum PlayerControlType { WASD, ARROW };

class Player : public Component
{
public:
  explicit Player(PlayerControlType controlType);

  void update(float dt) override;
  void fixedUpdate(float dt) override;

private:
  float speed;
  float jumpHeight;

  PlayerControlType controlType;

  std::weak_ptr<Transform> transform_ptr;
  std::weak_ptr<RigidBody> rigidBody_ptr;

  Vec3<float> appliedForce;

  void handleInput();
};


#endif //PLATFORMER_PLAYER_H
