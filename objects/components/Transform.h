#ifndef PLATFORMER_TRANSFORM_H
#define PLATFORMER_TRANSFORM_H

#include "Component.h"
#include "../../math/Vec2.h"
#include <vector>

class Transform : public Component
{
public:
  Transform(double xPos, double yPos, double width, double height);

  void fixedUpdate(double dt) override;

  double getX() const;
  double getY() const;

  void move(Vec2<double> vector);

  std::vector<Vec2<double>> getMesh() const;

  void reset();

private:
  std::vector<Vec2<double>> initialMesh;
  std::vector<Vec2<double>> oldMesh;
  std::vector<Vec2<double>> mesh;
  std::vector<Vec2<double>> newMesh;
};


#endif //PLATFORMER_TRANSFORM_H
