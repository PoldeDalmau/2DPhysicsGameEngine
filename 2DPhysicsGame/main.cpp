#include <SFML/Graphics.hpp>
#include "Circle.h"
#include "CircleManager.h"
#include "Car.h"
#include "Rectangle.h"
#include "Shape.h"
#include <iostream>

// Variable initializations
//const float deltaTime = 0.005f; // ensures car is stable.
const float deltaTime = 0.01f;
float floorRestitutionFactor = .90f;
float ShapeRestitutionFactor = .90f;
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
Rectangle rect(Point(700, 300), Point(-10, 0), 0, 10, sf::Color::White, 100, 300, 0/*M_PI/4.01*/, floorRestitutionFactor, ShapeRestitutionFactor, 0.3);
//Rectangle rect2(Point(200, 300), Point(0, 0/*5, 10*/), 1, 1, sf::Color::Green, 10, 300, 5, floorRestitutionFactor, ShapeRestitutionFactor, 0/*30*/);
Circle c1(0, 50, Point(300, 300), Point(-22, 33/*50, -6*/), floorRestitutionFactor, ShapeRestitutionFactor, 1);
Circle c2(1, 75, Point(400, 200), Point(45, 10), floorRestitutionFactor, ShapeRestitutionFactor, 2);
//Circle c3(2, 20, Point(600, 250), Point(0, 0));
//cman.AddCircle(c2);
//cman.AddCircle(c3);
sf::Font font;

int main()
{
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

    bool dummy = true;
    float jumpDist = 5;
    while (window.isOpen())
    {
        sf::Time elapsed1 = clock.getElapsedTime();

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            else if (event.type == sf::Event::KeyPressed) {
                //spin wheel
                if (event.key.code == sf::Keyboard::D) {
                    rect.position.x += (jumpDist);
                    //car.turnWheel(-1);
                }
                if (event.key.code == sf::Keyboard::A) {
                    rect.position.x += (-jumpDist);
                    //car.turnWheel(1);
                }
                if (event.key.code == sf::Keyboard::S)
                    rect.position.y += (jumpDist);
                if (event.key.code == sf::Keyboard::W)
                    rect.position.y += (-jumpDist);
                if (event.key.code == sf::Keyboard::Q)
                    rect.angle += (-jumpDist * M_PI / 180);
                if (event.key.code == sf::Keyboard::E)
                    rect.angle += (jumpDist * M_PI / 180);
                //if (event.key.code == sf::Keyboard::Space)
                //    car.jump();
                //if (event.key.code == sf::Keyboard::V)
                //    car.drawSprings();

                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
                    window.close();
            }
        }

        window.clear();

        //cman.handleSpringSystem(numColsMSS, numRowsMSS, latticeConstantMSS);
        //wallxVelocity = 0;

        c1.Gravity(9.81);
        c2.Gravity(9.81);

        if (c1.CheckCircleCircleCollision(c2)) {
            c1.ResolveCircleCircleCollision(c2);
        }
        c1.updatePostionVerlet(deltaTime);
        c2.updatePostionVerlet(deltaTime);
        //c3.updatePostionVerlet(deltaTime);

        c1.ResolveWallCollision(screenWidth, screenHeight);
        c2.ResolveWallCollision(screenWidth, screenHeight);
        //c3.ResolveWallCollision(screenWidth, screenHeight);
        c1.Draw(window);
        c2.Draw(window);


        //c3.Draw(window);
        //c2.Draw(window);
        //car.Gravity(10);
        //cman.Gravity(10);
        //cman.SpringLink(0, 1, 200, 10,0.5);
        //cman.UpdateAll(dummy);
        //car.updateCar();

        //car.DrawAll();
        //cman.DrawAll();
        sf::Time elapsed2 = clock.getElapsedTime();

        rect.Gravity(9.81);
        rect.update(deltaTime);
        //rect2.update(deltaTime);
        rect.Draw(window);
        //rect2.Draw(window);

        //if (rect.isNearRectangle(rect2))
        //    (rect.isRectangleCollsion(rect2));
        if(rect.isNearWall(screenWidth, screenHeight))
            rect.ResolveWallCollision(screenWidth, screenHeight);
        //if (rect2.isNearWall(screenWidth, screenHeight))
        //    rect2.ResolveWallCollision(screenWidth, screenHeight);

        if (rect.isNearCircle(c1))
            if (rect.isCircleCollsion(c1/*, window*/)) {
                rect.resolveCircleCollision(c1);
            }
        if (rect.isNearCircle(c2))
            if (rect.isCircleCollsion(c2)) {
                rect.resolveCircleCollision(c2);
            }
        //if (rect2.isNearCircle(c1))
        //    if (rect2.isCircleCollsion(c1/*, window*/)) {
        //        rect2.resolveCircleCollision(c1);
        //    }
        
        
        // output stats to sf window
        //cman.makeVelocityHistogram(30, 25);

        //stats = cman.getStats();
        sf::Text text;
        worldtime += deltaTime;
        text.setString("\n Time elapsed: " + std::to_string(worldtime));
        //text.setString("\nNumber of particles: " + stats[0] + 
        //    "\nInternal Energy:\n" + stats[1] + 
        //    "\nVelocity of red particle: \n" + stats[2] + 
        //    "\nFrame update time: " + std::to_string(elapsed2.asMilliseconds() - elapsed1.asMilliseconds()) + " ms");
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
