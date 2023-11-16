#ifndef PLATFORMER_PLAYER_H
#define PLATFORMER_PLAYER_H

#include "Component.h"

class GameObject;
class Transform;
class RigidBody;

enum PlayerControlType {
    WASD,
    ARROW
};

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

    Transform* transform;
    RigidBody* rigidBody;

    void handleInput();
};


#endif //PLATFORMER_PLAYER_H
