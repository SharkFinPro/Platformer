#ifndef PLATFORMER_RIGIDBODY_H
#define PLATFORMER_RIGIDBODY_H

#include "Component.h"
#include <vector>

class Transform;
class BoxCollider;

class RigidBody : public Component
{
public:
    RigidBody();

    void fixedUpdate(float dt) override;

    void applyForce(float x, float y);

    bool isFalling() const;

    void handleCollisions(const std::vector<GameObject*> &objects);

private:
    float xvel;
    float yvel;

    bool doGravity;
    float gravity;
    float maxFallSpeed;

    bool falling;

    Transform* transform;
    BoxCollider* boxCollider;

    void limitMovement();

    void handleXCollision();
    void handleYCollision();
};


#endif //PLATFORMER_RIGIDBODY_H
