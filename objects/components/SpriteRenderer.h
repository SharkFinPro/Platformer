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

  void update(float dt) override;

  void setColor(sf::Color color);

private:
  sf::Color color;
  std::shared_ptr<Transform> transform;
};


#endif //PLATFORMER_SPRITERENDERER_H
