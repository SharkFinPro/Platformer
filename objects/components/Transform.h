#ifndef PLATFORMER_TRANSFORM_H
#define PLATFORMER_TRANSFORM_H

#include "Component.h"
#include "../../math/Vec2.h"
#include <vector>

class Transform : public Component
{
public:
  Transform(float xPos, float yPos, float width, float height);

  void fixedUpdate(float dt) override;

  float getX() const;
  float getY() const;

  void move(Vec2<float> vector);

  std::vector<Vec2<float>> getMesh() const;

  void reset();

private:
  std::vector<Vec2<float>> initialMesh;
  std::vector<Vec2<float>> oldMesh;
  std::vector<Vec2<float>> mesh;
  std::vector<Vec2<float>> newMesh;
};


#endif //PLATFORMER_TRANSFORM_H
