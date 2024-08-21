#ifndef PLATFORMER_SIMPLEX_H
#define PLATFORMER_SIMPLEX_H

#include "../../../math/Vec3.h"

class Simplex
{
public:
  Simplex();

  [[nodiscard]] size_t size() const;

  [[nodiscard]] const Vec3<float>& getA() const;
  [[nodiscard]] const Vec3<float>& getB() const;
  [[nodiscard]] const Vec3<float>& getC() const;

  void removeB();
  void removeC();

  void addVertex(Vec3<float> vertex);

private:
  Vec3<float> vertices[3];
  size_t length;
};

#endif //PLATFORMER_SIMPLEX_H
