#include <SFML/Graphics.hpp>
#include "objects/ObjectManager.h"
#include "objects/Object.h"
#include "objects/components/RigidBody.h"
#include "objects/components/Player.h"
#include "objects/components/Transform.h"
#include "objects/components/SpriteRenderer.h"
#include "objects/components/collisions/MeshCollider.h"
#include <chrono>

std::vector<Vec2<float>> createQuadMesh(float width, float height)
{
  auto mesh = std::vector<Vec2<float>>{};
  mesh.emplace_back(-width / 2, -height / 2);
  mesh.emplace_back(width / 2, -height / 2);
  mesh.emplace_back(width / 2, height / 2);
  mesh.emplace_back(-width / 2, height / 2);

  return mesh;
}

Object* createPlayer(float x, float y, float width, float height, PlayerControlType controlType, sf::Color color)
{
  auto player = new Object;
  player->addComponent(new Transform{x, y, createQuadMesh(width, height)});
  player->addComponent(new RigidBody);
  player->addComponent(new Player{controlType});
  player->addComponent(new SpriteRenderer{color});
  player->addComponent(new MeshCollider);

  return player;
}

Object* createBlock(float x, float y, float width, float height, sf::Color color = sf::Color{125, 125, 125})
{
  auto obj = new Object;

  obj->addComponent(new Transform{x, y, createQuadMesh(width, height)});
  obj->addComponent(new MeshCollider);
  obj->addComponent(new SpriteRenderer{color});

  return obj;
}

Object* createRigidBlock(float x, float y, float width, float height, sf::Color color)
{
  auto obj = new Object;

  obj->addComponent(new Transform{x, y, createQuadMesh(width, height)});
  obj->addComponent(new MeshCollider);
  obj->addComponent(new RigidBody);
  obj->addComponent(new SpriteRenderer{color});

  return obj;
}

int main()
{
  // Create Entities
  ObjectManager objectManager;

  objectManager.addObject(createPlayer(200, 200, 50, 50, PlayerControlType::WASD, sf::Color{42, 139, 200}));
  objectManager.addObject(createPlayer(400, 400, 50, 50, PlayerControlType::ARROW, sf::Color{175, 75, 150}));

  objectManager.addObject(createRigidBlock(300, 200, 50, 50, sf::Color{240, 139, 100}));

  objectManager.addObject(createBlock(960, 1055, 1920, 50));
  objectManager.addObject(createBlock(900, 1005, 250, 50));

  for (int i = 0; i < 9; i++)
    objectManager.addObject(createBlock(static_cast<float>(i) * 50.0f + 75.0f, 925, 50, 50));

  for (int i = 0; i < 30; i++)
    objectManager.addObject(createBlock(100, static_cast<float>(i) * 50.0f + 75.0f, 50, 50));


  // Create the window
  sf::RenderWindow window(sf::VideoMode{1920, 1080}, "Platformer", sf::Style::None);
  window.setMouseCursorVisible(false);
  objectManager.setWindow(&window);

  std::chrono::steady_clock::time_point previous = std::chrono::steady_clock::now();

  // run the program as long as the window is open
  while (window.isOpen())
  {
    // check all the window's events that were triggered since the last iteration of the loop
    sf::Event event{};
    while (window.pollEvent(event))
      if (event.type == sf::Event::Closed)
        window.close();

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
      window.close();

    // Delta Time
    std::chrono::steady_clock::time_point current = std::chrono::steady_clock::now();
    float dt = static_cast<float>(std::chrono::duration_cast<std::chrono::microseconds>(current - previous).count()) / 1000000.0f;
    previous = current;

    // clear the window with black color
    window.clear(sf::Color{200, 200, 200});

    objectManager.update(dt);

    // end the current frame
    window.display();
  }
}