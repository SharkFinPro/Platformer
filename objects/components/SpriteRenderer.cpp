#include "SpriteRenderer.h"
#include "Transform.h"
#include "../GameObject.h"

SpriteRenderer::SpriteRenderer(sf::Color color)
    : Component{ComponentType::spriteRenderer}, color{color}
{}

void SpriteRenderer::draw(sf::RenderWindow* window)
{
    auto transform = dynamic_cast<Transform*>(owner->getComponent(ComponentType::transform));

    if (!transform)
        return;

    sf::RectangleShape shape = sf::RectangleShape({transform->getWidth(), transform->getHeight()});
    shape.move({transform->getX(), transform->getY()});
    shape.setFillColor(color);

    window->draw(shape);
}

void SpriteRenderer::setColor(sf::Color color)
{
    this->color = color;
}
