#ifndef PLATFORMER_SPRITERENDERER_H
#define PLATFORMER_SPRITERENDERER_H

#include "Component.h"
#include <SFML/Graphics.hpp>

class Transform;

class SpriteRenderer : public Component
{
public:
  explicit SpriteRenderer(sf::Color color);

  void update(double dt) override;

  void setColor(sf::Color color);

private:
  sf::Color color;
  Transform* transform;
};


#endif //PLATFORMER_SPRITERENDERER_H
