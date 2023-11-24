#ifndef PLATFORMER_VEC2_H
#define PLATFORMER_VEC2_H

#include <cmath>

template<typename T>
class Vec2
{
public:
  Vec2(T x, T y);

  T getX() const;
  T getY() const;

  void setX(T x);
  void setY(T y);

  T dot(Vec2<T> other) const;

  T length() const;

  Vec2<T> operator+(Vec2<T> const& other);
  Vec2<T> operator-(Vec2<T> const& other);
  Vec2<T> operator*(float scalar);

private:
  T x;
  T y;
};

template<typename T>
Vec2<T>::Vec2(T x, T y)
  : x{x}, y{y}
{}

template<typename T>
T Vec2<T>::getX() const
{
  return x;
}

template<typename T>
T Vec2<T>::getY() const
{
  return y;
}

template<typename T>
void Vec2<T>::setX(T x)
{
  this->x = x;
}

template<typename T>
void Vec2<T>::setY(T y)
{
  this->y = y;
}

template<typename T>
T Vec2<T>::dot(Vec2<T> other) const
{
  return x * other.x + y * other.y;
}

template<typename T>
T Vec2<T>::length() const
{
  return std::sqrt(dot(this));
}

template<typename T>
Vec2<T> Vec2<T>::operator+(const Vec2<T> &other)
  {
  return { x + other.x, y + other.y };
}

template<typename T>
Vec2<T> Vec2<T>::operator-(const Vec2<T> &other)
  {
  return { x - other.x, y - other.y };
}

template<typename T>
Vec2<T> Vec2<T>::operator*(const float scalar)
  {
  return { x * scalar, y * scalar };
}

#endif //PLATFORMER_VEC2_H
