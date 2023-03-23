#ifndef PLATFORMER_SPRITERENDERER_H
#define PLATFORMER_SPRITERENDERER_H

#include "Component.h"
#include <SFML/Graphics.hpp>

class SpriteRenderer : public Component
{
public:
    explicit SpriteRenderer(sf::Color color);

    void draw(sf::RenderWindow* window);

    void setColor(sf::Color color);

private:
    sf::Color color;
};


#endif //PLATFORMER_SPRITERENDERER_H
