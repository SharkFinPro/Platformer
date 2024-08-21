#ifndef PLATFORMER_SPRITERENDERER_H
#define PLATFORMER_SPRITERENDERER_H

#include "Component.h"
#include <SFML/Graphics.hpp>
#include <memory>

class Transform;

class SpriteRenderer final : public Component
{
public:
  explicit SpriteRenderer(sf::Color color);

  void update(float dt) override;

  [[maybe_unused]] void setColor(sf::Color color);

private:
  sf::ConvexShape shape;
  std::weak_ptr<Transform> transform_ptr;
};


#endif //PLATFORMER_SPRITERENDERER_H
