#include <SFML/Graphics.hpp>
#include "Circle.h"
#include "CircleManager.h"
#include "Car.h"
#include "Rectangle.h"
#include <iostream>

int main()
{
    // Variable initializations
    const float deltaTime = 0.001f;
    float screenHeight = 720;
    float screenWidth = 1280;
    float marginWidth = 280; // margin to display real time stats
    float wallxPosition = screenWidth - marginWidth;
    float wallxVelocity = 0;
    vector<std::string> stats;
    // Screen initialization
    sf::RenderWindow window(sf::VideoMode(screenWidth, screenHeight), "Physics SandBox");
    // Class initializations
    Car car(screenWidth, screenHeight, deltaTime, window, 0.1, 40);
    CircleManager cman(screenWidth, screenHeight, deltaTime, window);
    Rectangle rect(100, 300, 0, 0, 0, 1, sf::Color::White, 100, 100, 0, 0, 0);
    Rectangle rect2(200, 300, 0, 0, 1, 1, sf::Color::Green, 10, 300,0/*45*/,0,0);
    sf::Font font;

    if (!font.loadFromFile(("C:/Users/polde/OneDrive/Desktop/Projects/2DPhysicsGame/sfml/VCR_OSD_MONO_1.001.ttf"))) { // C:/Users/polde/OneDrive/Desktop/Projects/2DPhysicsGame
        // Handle font loading error
        std::cout << "Error loading .ttf file for font" << std::endl;
        system("pause");
    }


    //window.setFramerateLimit(64);

    sf::Clock clock;
    sf::Time elapsed2 = clock.getElapsedTime();

    //
    //int numRowsMSS = 3;
    //int numColsMSS = 3;
    //float latticeConstantMSS = 50;
    //float radiusMSS = 20;
    //float massMSS = 20;
    //cman.initializeMassSpringSystem(numColsMSS, numRowsMSS, latticeConstantMSS, radiusMSS, massMSS);

    bool dummy = false;

    while (window.isOpen())
    {
        sf::Time elapsed1 = clock.getElapsedTime();

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            else if (event.type == sf::Event::KeyPressed) {
                ////spin wheel
                if (event.key.code == sf::Keyboard::D)
                    rect.addxPosition(5);
                    //car.turnWheel(-1);
                if (event.key.code == sf::Keyboard::A)
                    rect.addxPosition(-5);
                    //car.turnWheel(1);
                if (event.key.code == sf::Keyboard::S)
                    rect.addyPosition(5);
                if (event.key.code == sf::Keyboard::W)
                    rect.addyPosition(-5);
                if (event.key.code == sf::Keyboard::Q)
                    rect.addAngle(-5);
                if (event.key.code == sf::Keyboard::E)
                    rect.addAngle(5);

                if (event.key.code == sf::Keyboard::Space)
                    car.jump();
                if (event.key.code == sf::Keyboard::V)
                    car.drawSprings();

                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
                    window.close();
            }
        }

        window.clear();

        //cman.handleSpringSystem(numColsMSS, numRowsMSS, latticeConstantMSS);
        //wallxVelocity = 0;

        //car.Gravity(25);
        ////cman.Gravity(10);
        ////cman.SpringLink(0, 1, 200, 10);
        ////cman.RodLink(0, 1, 200);
        ////cman.UpdateAll(dummy);
        //car.updateCar();

        //car.DrawAll();
        //cman.DrawAll();
        sf::Time elapsed2 = clock.getElapsedTime();

        bool rectangleCollision;
        if (rect.isNearRectangle(rect2))
            rect.isRectangleCollsion(rect2);
        if(rect.isNearWall(screenWidth, screenHeight))
            rect.ResolveWallCollision(screenWidth, screenHeight);
        if (rect2.isNearWall(screenWidth, screenHeight))
            rect2.ResolveWallCollision(screenWidth, screenHeight);

        rect.update(deltaTime);
        rect2.update(deltaTime);
        rect.Draw(window);
        rect2.Draw(window);
        // output stats to sf window
        //cman.makeVelocityHistogram(30, 25);

        //stats = cman.getStats();
        //sf::Text text;
        //text.setString("\nNumber of particles: " + stats[0] + 
        //    "\nInternal Energy:\n" + stats[1] + 
        //    "\nVelocity of red particle: \n" + stats[2] + 
        //    "\nFrame update time: " + std::to_string(elapsed2.asMilliseconds() - elapsed1.asMilliseconds()) + " ms");
        //text.setCharacterSize(16);
        //text.setFillColor(sf::Color::White);
        //text.setPosition(screenWidth - marginWidth + 25, 5.00f);
        //text.setFont(font);
        //window.draw(text);


        //window.draw(rectangle);
        window.display();
    }

    return 0;
}
