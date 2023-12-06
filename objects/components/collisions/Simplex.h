#ifndef PLATFORMER_SIMPLEX_H
#define PLATFORMER_SIMPLEX_H

#include "../../math/Vec3.h"

class Simplex
{
public:
  Simplex();

private:
  Vec3<float> vertices[3];
  size_t length;
};

#endif //PLATFORMER_SIMPLEX_H
