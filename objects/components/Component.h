#ifndef PLATFORMER_COMPONENT_H
#define PLATFORMER_COMPONENT_H

class GameObject;

class Component
{
public:
    explicit Component(const char* name);
    virtual ~Component() = default;

    const char* getName() const;

    void setOwner(GameObject* componentOwner);
    GameObject* getOwner() const;

    virtual void update(float dt);
    virtual void fixedUpdate(float dt);

protected:
    const char* name;
    GameObject* owner;
};

#endif //PLATFORMER_COMPONENT_H
