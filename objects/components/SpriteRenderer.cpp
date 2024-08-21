#include "SpriteRenderer.h"
#include "Transform.h"
#include "../Object.h"
#include "../ObjectManager.h"

SpriteRenderer::SpriteRenderer(sf::Color color)
  : Component{ComponentType::spriteRenderer}
{
  shape.setFillColor(color);
}

void SpriteRenderer::update([[maybe_unused]] const float dt)
{
  if (transform_ptr.expired())
  {
    transform_ptr = dynamic_pointer_cast<Transform>(owner->getComponent(ComponentType::transform));

    if (transform_ptr.expired())
    {
      return;
    }
  }

  if (const std::shared_ptr<Transform> transform = transform_ptr.lock())
  {
    const auto mesh = transform->getMesh();

    shape.setPointCount(mesh.size());
    for (int i = 0; i < static_cast<int>(mesh.size()); i++)
    {
      shape.setPoint(i, sf::Vector2f(mesh[i].getX(), mesh[i].getY()));
    }

    getOwner()->getOwner()->getWindow()->draw(shape);
  }
}

[[maybe_unused]] void SpriteRenderer::setColor(sf::Color color)
{
  shape.setFillColor(color);
}
