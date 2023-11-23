#ifndef PLATFORMER_BOXCOLLIDER_H
#define PLATFORMER_BOXCOLLIDER_H

#include "Component.h"
#include "../../math/Vec2.h"
#include <vector>

class GameObject;
class Transform;

struct BoundingRectangle {
  double left, right, top, bottom;

  double width() const {
    return right - left;
  }

  double height() const {
    return bottom - top;
  }
};

class BoxCollider : public Component
{
public:
  BoxCollider();

  bool collidesWith(BoundingRectangle other);

  Vec2<double> getPenetrationVector(const std::vector<GameObject*>& objects);

  static BoundingRectangle getBoundingRectangle(const std::vector<Vec2<double>>& mesh);

private:
  Transform* transform;

  Vec2<double> getPenetration(GameObject* object);
  static Vec2<double> getTheoreticalPenetration(BoundingRectangle boundingRectangle, GameObject* object);
  static Vec2<double> getActualPenetration(BoundingRectangle r1, BoundingRectangle r2);
};

#endif //PLATFORMER_BOXCOLLIDER_H
