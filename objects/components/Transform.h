#ifndef PLATFORMER_TRANSFORM_H
#define PLATFORMER_TRANSFORM_H

#include "Component.h"
#include "../../math/Vec3.h"
#include <vector>

class Transform : public Component
{
public:
  Transform(float x, float y, std::vector<Vec3<float>> mesh);

  [[nodiscard]] Vec3<float> getPosition() const;

  void move(const Vec3<float>& vector);

  [[nodiscard]] std::vector<Vec3<float>> getMesh() const;

  void reset();

private:
  Vec3<float> initialPosition;
  Vec3<float> position;

  std::vector<Vec3<float>> mesh;
};


#endif //PLATFORMER_TRANSFORM_H
