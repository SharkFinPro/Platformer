#ifndef PLATFORMER_PLAYER_H
#define PLATFORMER_PLAYER_H

#include "Component.h"
class GameObject;

class Player : public Component
{
public:
    Player();

    void update(float dt) override;
    void fixedUpdate(float dt) override;

private:
    float speed;
    float jumpHeight;

    void handleInput();
};


#endif //PLATFORMER_PLAYER_H
