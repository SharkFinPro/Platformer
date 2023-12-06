#ifndef PLATFORMER_VEC3_H
#define PLATFORMER_VEC3_H

#include <cmath>
#include "Vec2.h"

template<typename T>
class Vec3
{
public:
  Vec3();
  Vec3(T x, T y, T z);
  Vec3(Vec2<T> xy, T z);

  T getX() const;
  T getY() const;
  T getZ() const;

  void setX(T x);
  void setY(T y);
  void setZ(T z);

  T dot(Vec3<T> other) const;
  Vec3<T> cross(Vec3<T> other) const;

  T length() const;

  Vec3<T> normalized() const;

  Vec3<T> operator+(Vec3<T> const& other);
  Vec3<T> operator-(Vec3<T> const& other);
  Vec3<T> operator*(float scalar);

private:
  T x;
  T y;
  T z;
};

template<typename T>
Vec3<T>::Vec3()
  : x{0}, y{0}, z{0}
{}

template<typename T>
Vec3<T>::Vec3(T x, T y, T z)
  : x{x}, y{y}, z{z}
{}

template<typename T>
Vec3<T>::Vec3(Vec2<T> xy, T z)
  : x{xy.getX()}, y{xy.getY()}, z{z}
{}

template<typename T>
T Vec3<T>::getX() const
{
  return x;
}

template<typename T>
T Vec3<T>::getY() const
{
  return y;
}

template<typename T>
T Vec3<T>::getZ() const
{
  return z;
}

template<typename T>
void Vec3<T>::setX(T x)
{
  this->x = x;
}

template<typename T>
void Vec3<T>::setY(T y)
{
  this->y = y;
}

template<typename T>
void Vec3<T>::setZ(T z)
{
  this->z = z;
}

template<typename T>
T Vec3<T>::dot(Vec3<T> other) const
{
  return x * other.x + y * other.y + z * other.z;
}

template<typename T>
Vec3<T> Vec3<T>::cross(Vec3<T> other) const {
  return {
    y * other.z - z * other.y,
    z * other.x - x * other.z,
    x * other.y - y * other.x
  };
}

template<typename T>
T Vec3<T>::length() const
{
  return std::sqrt(dot(*this));
}

template<typename T>
Vec3<T> Vec3<T>::normalized() const
{
  T len = length();

  return Vec3<T>(x / len, y / len, z / len);
}

template<typename T>
Vec3<T> Vec3<T>::operator+(const Vec3<T> &other)
  {
  return { x + other.x, y + other.y, z + other.z };
}

template<typename T>
Vec3<T> Vec3<T>::operator-(const Vec3<T> &other)
  {
  return { x - other.x, y - other.y, z - other.z };
}

template<typename T>
Vec3<T> Vec3<T>::operator*(const float scalar)
  {
  return { x * scalar, y * scalar, z * scalar };
}

#endif //PLATFORMER_VEC3_H
