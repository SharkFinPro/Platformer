#include "Simplex.h"

Simplex::Simplex()
  : length(0)
{}

size_t Simplex::size() const
{
  return length;
}

void Simplex::removeB()
{
  if (length < 1)
  {
    return;
  }

  length--;
  vertices[1] = vertices[2];
}

void Simplex::removeC()
{
  if (length < 2)
  {
    return;
  }

  length--;
}

void Simplex::addVertex(Vec3<float> vertex)
{
  for (auto i = length; i > 0; i--)
  {
    vertices[i] = vertices[i - 1];
  }

  vertices[0] = vertex;
  length++;
}

const Vec3<float>& Simplex::getA() const
{
  return vertices[0];
}

const Vec3<float>& Simplex::getB() const
{
  return vertices[1];
}

const Vec3<float>& Simplex::getC() const
{
  return vertices[2];
}
