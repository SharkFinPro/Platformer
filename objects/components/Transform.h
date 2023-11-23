#ifndef PLATFORMER_TRANSFORM_H
#define PLATFORMER_TRANSFORM_H

#include "Component.h"
#include "../../math/Vec2.h"
#include <vector>

class Transform : public Component
{
public:
  Transform(float xPos, float yPos, std::vector<Vec2<float>> mesh);

  void fixedUpdate(float dt) override;

  Vec2<float> getPosition() const;

  void move(Vec2<float> vector);

  std::vector<Vec2<float>> getMesh() const;

  void reset();

private:
  Vec2<float> initialPosition;
  Vec2<float> position;
  Vec2<float> newPosition;

  std::vector<Vec2<float>> mesh;
};


#endif //PLATFORMER_TRANSFORM_H
