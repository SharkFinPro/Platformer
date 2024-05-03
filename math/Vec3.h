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
  explicit Vec3(T xyz);

  T getX() const;
  T getY() const;
  T getZ() const;

  Vec2<T> xy() const;

  void setX(T x);
  void setY(T y);
  void setZ(T z);

  T dot(Vec3<T> other) const;
  Vec3<T> cross(Vec3<T> other) const;

  T length() const;

  Vec3<T> normalized() const;

  Vec3<T> operator+(Vec3<T> const& other) const;
  Vec3<T> operator-(Vec3<T> const& other) const;
  Vec3<T> operator-() const;
  Vec3<T> operator*(float scalar) const;

  Vec3<T> operator+=(Vec3<T> const& other);
  Vec3<T> operator-=(Vec3<T> const& other);
  Vec3<T> operator*=(float scalar);

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
Vec3<T>::Vec3(T x_, T y_, T z_)
  : x{x_}, y{y_}, z{z_}
{}

template<typename T>
Vec3<T>::Vec3(Vec2<T> xy, T z_)
  : x{xy.getX()}, y{xy.getY()}, z{z_}
{}

template<typename T>
Vec3<T>::Vec3(T xyz)
    : x{xyz}, y{xyz}, z{xyz}
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
Vec2<T> Vec3<T>::xy() const
{
  return { x, y };
}

template<typename T>
void Vec3<T>::setX(T x_)
{
  x = x_;
}

template<typename T>
void Vec3<T>::setY(T y_)
{
  y = y_;
}

template<typename T>
void Vec3<T>::setZ(T z_)
{
  z = z_;
}

template<typename T>
T Vec3<T>::dot(Vec3<T> other) const
{
  return x * other.x + y * other.y + z * other.z;
}

template<typename T>
Vec3<T> Vec3<T>::cross(Vec3<T> other) const
{
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
Vec3<T> Vec3<T>::operator+(const Vec3<T>& other) const
{
  return { x + other.x, y + other.y, z + other.z };
}

template<typename T>
Vec3<T> Vec3<T>::operator-(const Vec3<T>& other) const
{
  return { x - other.x, y - other.y, z - other.z };
}

template<typename T>
Vec3<T> Vec3<T>::operator-() const
{
  return { -x, -y, -z };
}

template<typename T>
Vec3<T> Vec3<T>::operator*(const float scalar) const
{
  return { x * scalar, y * scalar, z * scalar };
}

template<typename T>
Vec3<T> Vec3<T>::operator+=(const Vec3<T>& other)
{
  x += other.x;
  y += other.y;
  z += other.z;

  return *this;
}

template<typename T>
Vec3<T> Vec3<T>::operator-=(const Vec3<T>& other)
{
  x -= other.x;
  y -= other.y;
  z -= other.z;

  return *this;
}

template<typename T>
Vec3<T> Vec3<T>::operator*=(float scalar)
{
  x *= scalar;
  y *= scalar;
  z *= scalar;

  return *this;
}

#endif //PLATFORMER_VEC3_H
