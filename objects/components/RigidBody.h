#ifndef PLATFORMER_RIGIDBODY_H
#define PLATFORMER_RIGIDBODY_H

#include "Component.h"
#include <vector>

class RigidBody : public Component
{
public:
    RigidBody();

    void fixedUpdate(float dt) override;

    float getXvel() const;
    float getYvel() const;
    void setXvel(float velocity);
    void setYvel(float velocity);

    bool isFalling() const;

    void handleCollision(GameObject* other, float dt);

private:
    float xvel;
    float yvel;

    bool doGravity;
    float gravity;
    float maxFallSpeed;

    bool falling;
    bool collided;

    void limitMovement();
};


#endif //PLATFORMER_RIGIDBODY_H
