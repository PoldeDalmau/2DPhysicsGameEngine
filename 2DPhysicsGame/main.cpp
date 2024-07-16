#include <SFML/Graphics.hpp>
#include "Circle.h"
#include "CircleManager.h"
#include "Car.h"
#include "Rectangle.h"
#include "Shape.h"
#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

// Variable initializations
//const float deltaTime = 0.005f; // ensures car is stable.
const float deltaTime = 0.01f;
float floorRestitutionFactor = .50f;
float ShapeRestitutionFactor = .50f;
float worldtime = 0;
float screenHeight = 720;
float screenWidth = 1280;
float marginWidth = 280; // margin to display real time stats
float wallxPosition = screenWidth - marginWidth;
float wallxVelocity = 0;
vector<std::string> stats;
// Screen initialization
sf::RenderWindow window(sf::VideoMode(screenWidth, screenHeight), "Physics SandBox");
// Class initializations
//Car car(screenWidth, screenHeight, deltaTime, window, 1, 40);
CircleManager cman(screenWidth, screenHeight, deltaTime, window);
Rectangle rect(Point(700, 300), Point(-40, 23), 0, 1, sf::Color::White, 100, 300, 0 /*M_PI/4.01*/, floorRestitutionFactor, ShapeRestitutionFactor, .0f);
Rectangle rect2(Point(200, 300), Point(0, 0), 1, 1, sf::Color::Blue, 500, 100, 0.1, floorRestitutionFactor, ShapeRestitutionFactor, 0/*30*/);
//Circle c1(0, 50, Point(300, 300), Point(-22, 33/*50, -6*/), floorRestitutionFactor, ShapeRestitutionFactor, 1);
//Circle c2(1, 75, Point(400, 200), Point(45, 10), floorRestitutionFactor, ShapeRestitutionFactor, 2);
//Circle c3(2, 20, Point(600, 250), Point(0, 0));
//cman.AddCircle(c2);
//cman.AddCircle(c3);
sf::Font font;

int main()
{
    fs::path currentPath = fs::current_path(); // Get the current directory path
    fs::path parentPath = currentPath.parent_path(); // Get the parent directory's path
    
    if (!font.loadFromFile((parentPath / "sfml/VCR_OSD_MONO_1.001.ttf").string())) {
        // Handle font loading error
        std::cout << "Error loading .ttf file for font" << std::endl;
        std::cout << "Make sure the file exists in" << std::endl;
        std::cout << parentPath / "sfml/VCR_OSD_MONO_1.001.ttf" << std::endl;
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
    cman.AddCirclesMesh(5, 300, 50);

    bool dummy = false;
    float jumpDist = 5;
    while (window.isOpen())
    {
        sf::Time elapsed1 = clock.getElapsedTime();

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            //else if (event.type == sf::Event::KeyPressed) {
            //    //spin wheel
            //    if (event.key.code == sf::Keyboard::D) {
            //        rect.position.x += (jumpDist);
            //        //car.turnWheel(-1);
            //    }
            //    if (event.key.code == sf::Keyboard::A) {
            //        rect.position.x += (-jumpDist);
            //        //car.turnWheel(1);
            //    }
            //    if (event.key.code == sf::Keyboard::S)
            //        rect.position.y += (jumpDist);
            //    if (event.key.code == sf::Keyboard::W)
            //        rect.position.y += (-jumpDist);
            //    if (event.key.code == sf::Keyboard::Q)
            //        rect.angle += (-jumpDist * M_PI / 180);
            //    if (event.key.code == sf::Keyboard::E)
            //        rect.angle += (jumpDist * M_PI / 180);
            //    //if (event.key.code == sf::Keyboard::Space)
            //    //    car.jump();
            //    //if (event.key.code == sf::Keyboard::V)
            //    //    car.drawSprings();

                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
                    window.close();
            //}
        }

        window.clear();

        cman.UpdateAll(dummy);
        cman.DrawAll();
        sf::Time elapsed2 = clock.getElapsedTime();
        
        // output stats to sf window
        //cman.makeVelocityHistogram(30, 25);

        stats = cman.getStats();

        sf::Text text;
        worldtime += deltaTime;
        text.setString("\n Time elapsed: " + std::to_string(worldtime));
        text.setString("\nNumber of particles: " + stats[0] + 
            "\nInternal Energy:\n" + stats[1] + 
            "\nVelocity of red particle: \n" + stats[2] + 
            "\nFrame update time: " + std::to_string(elapsed2.asMilliseconds() - elapsed1.asMilliseconds()) + " ms");
        text.setCharacterSize(16);
        text.setFillColor(sf::Color::White);
        text.setPosition(screenWidth - marginWidth + 25, 5.00f);
        text.setFont(font);
        window.draw(text);


        //window.draw(rectangle);
        window.display();
    }

    return 0;
}
