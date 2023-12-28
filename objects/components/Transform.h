#ifndef PLATFORMER_TRANSFORM_H
#define PLATFORMER_TRANSFORM_H

#include "Component.h"
#include "../../math/Vec3.h"
#include <vector>

class Transform : public Component
{
public:
  Transform(float xPos, float yPos, std::vector<Vec3<float>> mesh);

  void fixedUpdate(float dt) override;

  [[nodiscard]] Vec3<float> getPosition() const;

  void move(Vec3<float> vector);

  [[nodiscard]] std::vector<Vec3<float>> getMesh() const;

  void reset();

private:
  Vec3<float> initialPosition;
  Vec3<float> position;
  Vec3<float> newPosition;

  std::vector<Vec3<float>> mesh;
};


#endif //PLATFORMER_TRANSFORM_H
