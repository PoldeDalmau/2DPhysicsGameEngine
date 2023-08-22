#include <SFML/Graphics.hpp>
#include "Circle.h"
#include "CollisionHandler.h"

int main()
{
    float deltaTime = 0.01f;

    CollisionHandler ch;
    Circle circle1(50, 100, 150, 0, 0);
    Circle circle2(50, 700, 150, 20, -12);

    float screenWidth = 1280;
    float screenHeight = 720;

    sf::RenderWindow window(sf::VideoMode(screenWidth, screenHeight), "Zocker Physics");

    window.setFramerateLimit(120);


    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
                window.close();
        }

        window.clear();

        // handle all updates:
        
        circle1.updatePosition(.5f);
        circle2.updatePosition(.5f);

        if (ch.CheckCircleCircleCollision(circle1, circle2)) {
            ch.ResolveCircleCircleCollision(circle1, circle2);
        }
        ch.ResolveCircleWallCollision(circle1, screenWidth, screenHeight);
        ch.ResolveCircleWallCollision(circle2, screenWidth, screenHeight);

        circle1.Draw(window);
        circle2.Draw(window);
        window.display();
    }

    return 0;
}
