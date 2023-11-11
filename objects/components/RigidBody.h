#ifndef PLATFORMER_RIGIDBODY_H
#define PLATFORMER_RIGIDBODY_H

#include "Component.h"
#include <vector>

class RigidBody : public Component
{
public:
    RigidBody();

    void fixedUpdate(float dt) override;

    void setXvel(float velocity);
    void setYvel(float velocity);

    bool isFalling() const;

    void handleCollisions(const std::vector<GameObject*> &objects);

private:
    float xvel;
    float yvel;

    bool doGravity;
    float gravity;
    float maxFallSpeed;

    bool falling;

    void limitMovement();
};


#endif //PLATFORMER_RIGIDBODY_H
