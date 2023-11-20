#ifndef PLATFORMER_BOXCOLLIDER_H
#define PLATFORMER_BOXCOLLIDER_H

#include "Component.h"
#include "../../math/Vec2.h"
#include <vector>

struct BoundingRectangle;
class GameObject;
class Transform;

class BoxCollider : public Component
{
public:
  BoxCollider();

  bool collidesWith(BoundingRectangle other);

  Vec2<float> getPenetrationVector(const std::vector<GameObject*>& objects);

private:
  Transform* transform;

  Vec2<float> getPenetration(GameObject* object);
  static Vec2<float> getTheoreticalPenetration(BoundingRectangle boundingRectangle, GameObject* object);
  static Vec2<float> getActualPenetration(BoundingRectangle r1, BoundingRectangle r2);
};

#endif //PLATFORMER_BOXCOLLIDER_H
