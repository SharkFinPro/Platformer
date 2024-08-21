#include <SFML/Graphics.hpp>
#include "objects/ObjectManager.h"
#include "objects/Object.h"
#include "objects/components/Components.h"
#include <chrono>
#include <memory>

std::vector<Vec3<float>> createQuadMesh(float width, float height);
std::shared_ptr<Object> createPlayer(float x, float y, float width, float height, PlayerControlType controlType, sf::Color color);
std::shared_ptr<Object> createBlock(float x, float y, float width, float height, sf::Color color = sf::Color{125, 125, 125});
std::shared_ptr<Object> createRigidBlock(float x, float y, float width, float height, sf::Color color);

void loadObjects (ObjectManager& objectManager);

int main()
{
  // Create Entities
  ObjectManager objectManager;
  loadObjects(objectManager);

  // Create the window
  sf::RenderWindow window(sf::VideoMode{1920, 1080}, "Platformer", sf::Style::None);
  window.setMouseCursorVisible(false);
  objectManager.setWindow(&window);

  std::chrono::steady_clock::time_point previous = std::chrono::steady_clock::now();

  // Run the program as long as the window is open
  while (window.isOpen())
  {
    // Check all the window's events that were triggered since the last iteration of the loop
    sf::Event event{};
    while (window.pollEvent(event))
    {
      if (event.type == sf::Event::Closed)
      {
        window.close();
      }
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
    {
      window.close();
    }

    // Clear the window with a light gray color
    window.clear(sf::Color{200, 200, 200});

    // Delta time based off previous frame
    std::chrono::steady_clock::time_point current = std::chrono::steady_clock::now();
    float dt = static_cast<float>(std::chrono::duration_cast<std::chrono::microseconds>(current - previous).count()) / 1000000.0f;
    previous = current;

    objectManager.update(dt);

    // Display the current frame
    window.display();
  }
}

void loadObjects (ObjectManager& objectManager)
{
  objectManager.addObject(createPlayer(200, 200, 50, 50, PlayerControlType::WASD, sf::Color{42, 139, 200}));
  objectManager.addObject(createPlayer(400, 400, 50, 50, PlayerControlType::ARROW, sf::Color{175, 75, 150}));

  objectManager.addObject(createRigidBlock(300, 200, 50, 50, sf::Color{240, 139, 100}));

  objectManager.addObject(createBlock(960, 1055, 1920, 50));
  objectManager.addObject(createBlock(900, 1005, 250, 50));

  for (int i = 0; i < 9; i++)
  {
    objectManager.addObject(createBlock(static_cast<float>(i) * 50.0f + 75.0f, 925, 50, 50));
  }

  for (int i = 0; i < 30; i++)
  {
    objectManager.addObject(createBlock(100, static_cast<float>(i) * 50.0f + 75.0f, 50, 50));
  }
}

std::vector<Vec3<float>> createQuadMesh(float width, float height)
{
  auto mesh = std::vector<Vec3<float>>{};
  mesh.emplace_back(-width / 2, -height / 2, 0);
  mesh.emplace_back(width / 2, -height / 2, 0);
  mesh.emplace_back(width / 2, height / 2, 0);
  mesh.emplace_back(-width / 2, height / 2, 0);

  return mesh;
}

std::shared_ptr<Object> createPlayer(float x, float y, float width, float height, PlayerControlType controlType, sf::Color color)
{
  auto object = std::make_shared<Object>();

  object->addComponent(std::make_shared<Transform>(x, y, createQuadMesh(width, height)));
  object->addComponent(std::make_shared<RigidBody>());
  object->addComponent(std::make_shared<Player>(controlType));
  object->addComponent(std::make_shared<SpriteRenderer>(color));
  object->addComponent(std::make_shared<MeshCollider>());

  return object;
}

std::shared_ptr<Object> createBlock(float x, float y, float width, float height, sf::Color color)
{
  auto object = std::make_shared<Object>();

  object->addComponent(std::make_shared<Transform>(x, y, createQuadMesh(width, height)));
  object->addComponent(std::make_shared<MeshCollider>());
  object->addComponent(std::make_shared<SpriteRenderer>(color));

  return object;
}

std::shared_ptr<Object> createRigidBlock(float x, float y, float width, float height, sf::Color color)
{
  auto object = std::make_shared<Object>();

  object->addComponent(std::make_shared<Transform>(x, y, createQuadMesh(width, height)));
  object->addComponent(std::make_shared<MeshCollider>());
  object->addComponent(std::make_shared<RigidBody>());
  object->addComponent(std::make_shared<SpriteRenderer>(color));

  return object;
}