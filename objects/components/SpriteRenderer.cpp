#include "SpriteRenderer.h"
#include "Transform.h"
#include "../GameObject.h"
#include "../GameObjectManager.h"

SpriteRenderer::SpriteRenderer(sf::Color color)
  : Component{ComponentType::spriteRenderer}, color{color}, transform{nullptr}
{}

void SpriteRenderer::update([[maybe_unused]] float dt)
{
  if (!transform)
    transform = dynamic_cast<Transform*>(owner->getComponent(ComponentType::transform));

  if (!transform)
    return;

  auto mesh = transform->getMesh();
  sf::ConvexShape shape;
  shape.setPointCount(mesh.size());
  for (int i = 0; i < static_cast<int>(mesh.size()); i++)
    shape.setPoint(i, sf::Vector2f(mesh[i].getX(), mesh[i].getY()));
  
  shape.setFillColor(color);

  getOwner()->getOwner()->getWindow()->draw(shape);
}

void SpriteRenderer::setColor(sf::Color color)
{
  this->color = color;
}
