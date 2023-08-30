#include <SFML/Graphics.hpp>
#include "Circle.h"
#include "CircleManager.h"
#include <iostream>

int main()
{
    // Variable initializations
    float deltaTime = 0.01f;
    float screenHeight = 720;
    float screenWidth = 1280;
    float marginWidth = 280; // margin to display real time stats
    vector<std::string> stats;
    // Class initializations
    CircleManager cman(screenWidth - marginWidth, screenHeight);
    cman.AddCirclesMesh(screenHeight, screenWidth - marginWidth, 4, 41, 20);

    sf::RenderWindow window(sf::VideoMode(screenWidth, screenHeight), "Zocker Physics");

    window.setFramerateLimit(64);

    sf::Clock clock;
    sf::Time elapsed2 = clock.getElapsedTime();

    while (window.isOpen())
    {
        sf::Time elapsed1 = clock.getElapsedTime();

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
                window.close();
        }

        window.clear();

        // write some text
        sf::Font font;

        if (!font.loadFromFile(("C:/Users/polde/OneDrive/Desktop/Projects/2DPhysicsGame/sfml/VCR_OSD_MONO_1.001.ttf"))) { // C:/Users/polde/OneDrive/Desktop/Projects/2DPhysicsGame
            // Handle font loading error
            std::cout << "Error loading .ttf file for font" << std::endl;
            system("pause");
        }

        stats = cman.getStats();

        // handle all updates:
        cman.makeVelocityHistogram(window, 40, 250);
        cman.CheckCollisionsAndResolve();
        cman.UpdateAll(deltaTime);
        cman.DrawAll(window);
        sf::Time elapsed2 = clock.getElapsedTime();

        // output stats to sf window
        sf::Text text;
        text.setString("\nNumber of particles: " + stats[0] + 
            "\nInternal Energy:\n" + stats[1] + 
            "\nVelocity of red particle: \n" + stats[2] + 
            "\nFrame update time: " + std::to_string(elapsed2.asMilliseconds() - elapsed1.asMilliseconds()) + " ms");
        text.setCharacterSize(16);
        text.setFillColor(sf::Color::White);
        text.setPosition(screenWidth - marginWidth + 25, 5.00f);
        text.setFont(font);
        window.draw(text);


        sf::RectangleShape rectangle(sf::Vector2f(1, screenHeight));
        rectangle.setFillColor(sf::Color::White);
        rectangle.setPosition(sf::Vector2f(screenWidth - marginWidth, 0));
        window.draw(rectangle);
        window.display();
    }

    return 0;
}
