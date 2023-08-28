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
    vector<std::string> stats;
    // Class initializations
    CircleManager cman(screenWidth - 280, screenHeight);
    cman.AddCirclesMesh(screenHeight, screenWidth - 280, 4, 41, 12);

    sf::RenderWindow window(sf::VideoMode(screenWidth, screenHeight), "Zocker Physics");

    window.setFramerateLimit(32);


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

        // write some text
        sf::Font font;

        if (!font.loadFromFile(("C:/Users/polde/OneDrive/Desktop/Projects/2DPhysicsGame/sfml/VCR_OSD_MONO_1.001.ttf"))) { // C:/Users/polde/OneDrive/Desktop/Projects/2DPhysicsGame
            // Handle font loading error
            std::cout << "Error loading .ttf file for font" << std::endl;
            system("pause");
        }

        sf::Text text;
        text.setFont(font);
        stats = cman.getStats();
        text.setString("Hello, SFML!\nnumber of particles: " + stats[0] + "\ntemperature: " + stats[1] + "\nvelocity magnitude: \n" + stats[2]);
        text.setCharacterSize(18);
        text.setFillColor(sf::Color::White);
        text.setPosition(1010, 5.00f);


        // handle all updates:
        cman.makeVelocityHistogram(window, 50, 20000);
        cman.CheckCollisionsAndResolve();
        cman.UpdateAll(deltaTime);
        cman.DrawAll(window);
        window.draw(text);
        window.display();
    }

    return 0;
}
