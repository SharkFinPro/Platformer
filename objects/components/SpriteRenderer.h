#ifndef PLATFORMER_SPRITERENDERER_H
#define PLATFORMER_SPRITERENDERER_H

#include "Component.h"
#include <SFML/Graphics.hpp>
#include <memory>

class Transform;

class SpriteRenderer : public Component
{
public:
  explicit SpriteRenderer(sf::Color color);

  void update(const float& dt) override;

  [[maybe_unused]] void setColor(sf::Color color);

private:
  sf::Color color;
  std::weak_ptr<Transform> transform_ptr;
};


#endif //PLATFORMER_SPRITERENDERER_H
