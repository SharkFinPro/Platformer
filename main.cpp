#include <SFML/Graphics.hpp>

#include "objects/GameObjectManager.h"

#include "objects/GameObject.h"
#include "objects/components/BoxCollider.h"
#include "objects/components/RigidBody.h"
#include "objects/components/Player.h"
#include "objects/components/Transform.h"
#include "objects/components/SpriteRenderer.h"

GameObject* createPlayer()
{
    auto player = new GameObject;
    player->addComponent(new Transform{200, 200, 50, 50});
    player->addComponent(new BoxCollider);
    player->addComponent(new RigidBody);
    player->addComponent(new Player);
    player->addComponent(new SpriteRenderer{sf::Color{42, 139, 200}});

    return player;
}

GameObject* createBlock(float x, float y, float width, float height)
{
    auto obj = new GameObject;

    obj->addComponent(new Transform{x, y, width, height});
    obj->addComponent(new BoxCollider);
    obj->addComponent(new SpriteRenderer{sf::Color{125, 125, 125}});

    return obj;
}

GameObject* createRigidBlock(float x, float y, float width, float height)
{
    auto obj = new GameObject;

    obj->addComponent(new Transform{x, y, width, height});
    obj->addComponent(new BoxCollider);
    obj->addComponent(new RigidBody);
    obj->addComponent(new SpriteRenderer{sf::Color{175, 75, 150}});

    return obj;
}

int main()
{
    // Create Entities
    GameObjectManager gameObjectManager;

    gameObjectManager.addObject(createPlayer());

    gameObjectManager.addObject(createBlock(0, 1030, 1920, 50));

    for (int i = 0; i < 9; i++)
        gameObjectManager.addObject(createBlock(static_cast<float>(i) * 200.0f + 75.0f, 900, 50, 50));

    gameObjectManager.addObject(createRigidBlock(400, 400, 50, 50));

    // Create the window
    sf::ContextSettings settings;
    settings.majorVersion = 4;
    settings.minorVersion = 6;
    settings.antialiasingLevel = 16;
//    sf::RenderWindow window(sf::VideoMode{1000, 800}, "Platformer", sf::Style::None, settings);
    sf::RenderWindow window(sf::VideoMode{1920, 1080}, "Platformer", sf::Style::None, settings);
//    sf::RenderWindow window(sf::VideoMode{1920, 1080}, "Platformer", sf::Style::Fullscreen, settings);
    window.setMouseCursorVisible(false);
    gameObjectManager.setWindow(&window);

    sf::Clock deltaClock;
    sf::Time previous = deltaClock.getElapsedTime();

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
        sf::Time current = deltaClock.getElapsedTime();
        auto dt = static_cast<float>((current - previous).asMicroseconds()) / 10000.0f;
        previous = current;

        // clear the window with black color
        window.clear(sf::Color{200, 200, 200});

        gameObjectManager.update(dt);

        // end the current frame
        window.display();
    }
}