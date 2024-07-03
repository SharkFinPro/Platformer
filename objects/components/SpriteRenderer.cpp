#include "SpriteRenderer.h"
#include "Transform.h"
#include "../Object.h"
#include "../ObjectManager.h"

SpriteRenderer::SpriteRenderer(sf::Color color_)
  : Component{ComponentType::spriteRenderer}, color{color_}
{}

void SpriteRenderer::update([[maybe_unused]] const float dt)
{
  if (transform_ptr.expired())
  {
    transform_ptr = dynamic_pointer_cast<Transform>(owner->getComponent(ComponentType::transform));

    if (transform_ptr.expired())
      return;
  }

  if (std::shared_ptr<Transform> transform = transform_ptr.lock())
  {
    auto mesh = transform->getMesh();
    sf::ConvexShape shape;
    shape.setPointCount(mesh.size());
    for (int i = 0; i < static_cast<int>(mesh.size()); i++)
      shape.setPoint(i, sf::Vector2f(mesh[i].getX(), mesh[i].getY()));

    shape.setFillColor(color);

    getOwner()->getOwner()->getWindow()->draw(shape);
  }
}

[[maybe_unused]] void SpriteRenderer::setColor(sf::Color color_)
{
  this->color = color_;
}
