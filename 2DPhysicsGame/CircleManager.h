#pragma once
#include "Circle.h"
#include <vector>
#include <cassert>
#include <math.h>

// Use (void) to silence unused warnings.
#define assertm(exp, msg) assert(((void)msg, exp))

using std::vector;

class CircleManager {
private:
    vector<Circle> circles;
    float screenWidth;
    float screenHeight;
    vector<float> velocityDistribution;
public:
    CircleManager(float screenWidth, float screenHeight) : screenWidth(screenWidth), screenHeight(screenHeight) {}

    Circle circleAt(int circleIndex) {
        return circles[circleIndex];
    }

    void AddCircle(Circle circle){
        circles.push_back(circle);        
    }

    //I want to make a class that inherits from CircleManager and is specific for spring systems:
    //Problems to think about:
    //1. collisions between normal circles and circles that are part of the new class
    //2. maybe turn off collisions within mass system to save on computation power
    void initializeMassSpringSystem(int numCirclesRow, int numCirclesColumn, float latticeConstant, float radius) {
        assert(latticeConstant > 2 * radius, "Lattice Constant too small!");
        float xPos = 4 * radius;
        float yPos = 4 * radius;
        for (int i = 0; i < numCirclesRow; i++)
            for (int j = 0; j < numCirclesColumn; j++) 
                AddCircle(Circle(radius, xPos + i * latticeConstant, yPos + j * latticeConstant, j * 20, 0));
    }

    void drawSpring(sf::RenderWindow& window, int startIndex, int endIndex)
    {// draw vertices
        sf::VertexArray line(sf::Lines, 2);
        line[0].color = sf::Color::White;
        line[0].position = sf::Vector2f(circles[startIndex].getxPosition(), circles[startIndex].getyPosition());
        line[1].color = sf::Color::White;
        line[1].position = sf::Vector2f(circles[endIndex].getxPosition(), circles[endIndex].getyPosition());
        window.draw(line);
    }
    void handleSpringSystem(int numCirclesRow, int numCirclesColumn, float latticeConstant, float deltaTime, sf::RenderWindow& window) {
        for (int i = 1; i < numCirclesRow - 1; i++)
            for (int j = 1; j < numCirclesColumn - 1; j++) {
                //if (i > 1 && i < numCirclesRow - 1) {


                //SpringLink(i + j, (i - 1) * numCirclesRow + j, latticeConstant, deltaTime); // left neighbor
                //drawSpring(window, i + j, (i - 1) * numCirclesRow + j);

                //SpringLink(i + j, i + (j - numCirclesRow), latticeConstant, deltaTime); // above neighbor
                //SpringLink(i + j, (i - 1) + (j - numCirclesRow), latticeConstant, deltaTime); // above left neightbor
                //SpringLink(i + j, (i + numCirclesRow) + j, latticeConstant, deltaTime); // right neighbor
                
                //}
                //if (j > 1 && j < numCirclesColumn - 1) {
                //    SpringLink(i + j, i + (j - numCirclesColumn), latticeConstant, deltaTime);
                //    SpringLink(i + j, i + (j + numCirclesColumn), latticeConstant, deltaTime);
                //}
            }
    
        //0 3 6 9
        //1 4 7 10
        //2 5 8 11
        int i = 0;
        int j = 1;
        SpringLink(i, j, latticeConstant, deltaTime);
        drawSpring(window, i, j);
        SpringLink(i, j, latticeConstant, deltaTime);
        i = 1;
        j = 2;
        drawSpring(window, i, j);
        SpringLink(i, j, latticeConstant, deltaTime);
        i = 0;
        j = numCirclesRow;
        drawSpring(window, i, j);
        SpringLink(i, j, latticeConstant, deltaTime);
        i = 1;
        j = numCirclesRow + 1;
        drawSpring(window, i, j);
        SpringLink(i, j, latticeConstant, deltaTime);
        i = 2;
        j = numCirclesRow + 2;
        drawSpring(window, i, j);
        SpringLink(i, j, latticeConstant, deltaTime);
        i = 3;
        j = 4;
        drawSpring(window, i, j);
        SpringLink(i, j, latticeConstant, deltaTime);
        i = 4;
        j = 5;
        drawSpring(window, i, j);
        SpringLink(i, j, latticeConstant, deltaTime);
        i = 0;
        j = 4;
        drawSpring(window, i, j);
        SpringLink(i, j, latticeConstant * sqrt(2), deltaTime);
        i = 1;
        j = 3;
        drawSpring(window, i, j);
        SpringLink(i, j, latticeConstant * sqrt(2), deltaTime);
        i = 1;
        j = 5;
        drawSpring(window, i, j);
        SpringLink(i, j, latticeConstant * sqrt(2), deltaTime);
        i = 2;
        j = 4;
        drawSpring(window, i, j);
        SpringLink(i, j, latticeConstant * sqrt(2), deltaTime);

        // second column
        i = 3;
        j = 6;
        drawSpring(window, i, j);
        SpringLink(i, j, latticeConstant, deltaTime);
        i = 4;
        j = 7;
        drawSpring(window, i, j);
        SpringLink(i, j, latticeConstant, deltaTime);
        i = 5;
        j = 8;
        drawSpring(window, i, j);
        SpringLink(i, j, latticeConstant, deltaTime);
        i = 3;
        j = 7;
        drawSpring(window, i, j);
        SpringLink(i, j, latticeConstant * sqrt(2), deltaTime);
        i = 4;
        j = 6;
        drawSpring(window, i, j);
        SpringLink(i, j, latticeConstant * sqrt(2), deltaTime);
        i = 4;
        j = 8;
        drawSpring(window, i, j);
        SpringLink(i, j, latticeConstant * sqrt(2), deltaTime);
        i = 5;
        j = 7;
        drawSpring(window, i, j);
        SpringLink(i, j, latticeConstant * sqrt(2), deltaTime);

        i = 6;
        j = 7;
        drawSpring(window, i, j);
        SpringLink(i, j, latticeConstant, deltaTime);
        i = 7;
        j = 8;
        drawSpring(window, i, j);
        SpringLink(i, j, latticeConstant, deltaTime);
    }

    void AddCirclesMesh(float ScreenHeight, float ScreenWidth, float radius, int numberOfCircles, float latticeConstant) {
        assert(latticeConstant > 2 * radius, "Lattice Constant too small!");
        assert(numberOfCircles * latticeConstant * latticeConstant < ScreenHeight * ScreenWidth, "All these circles will not fit");
        assert(numberOfCircles > 0, "Number of Circles must be Natural!");

        int numberOfCirclesPerRow = int(ScreenWidth/latticeConstant);

        for (int numy = 0; numy < 10; numy++)
        for (int numx = 0; numx < numberOfCirclesPerRow; numx++) {
            //if (numberOfCircles == numx)
            //    break;
            float x = latticeConstant/2 + (numx * latticeConstant);
            float y = latticeConstant + 3 * (numy * latticeConstant);
            float vx = 0;
            float vy = 0;
            //float vx = -100 + 5*numx;
            //float vy = 100 - 5*numx;
            AddCircle(Circle(radius, x, y, vx, vy));
        }
        // make one circle to hit all the circles;
        AddCircle(Circle(radius, screenWidth / 2.0f + radius, ScreenHeight - radius, 2000, -100, sf::Color::Red));
    }

    // gravity
    // g: acceleration
    void Gravity(float g, float deltaTime) {
        for (Circle & circle : circles){
            circle.addyVelocity(g * deltaTime); // down is up in sfml
        }

    }

    // Adds a spring force between two circles.
    // k: spring constant
    // maxSeparation: threshold on distance, i.e. beyond that distance, k becomes effectively infinite.
    void SpringLink(int circle1Index, int circle2Index, float eqDistance, float deltaTime, float k = 10) {
        assert(circle1Index < circles.size(), "circle index out of bounds");
        assert(circle2Index < circles.size(), "circle index out of bounds");

        float distance = circles[circle1Index].getDistance(circles[circle2Index]);
        float distanceX = circles[circle2Index].getxPosition() - circles[circle1Index].getxPosition();
        float distanceY = circles[circle2Index].getyPosition() - circles[circle1Index].getyPosition();
        // Hook's Law
        // harmonic potential around the equilibrium distance eqDistance
        float damping = 0.01;
        float forceX = k * distanceX * (1 - eqDistance / distance) * deltaTime;
        float forceY = k * distanceY * (1 - eqDistance / distance) * deltaTime;
        // Update velocities according to Hook
        circles[circle1Index].addxVelocity(forceX/*/ circles[circle1Index].getMass()*/ - damping * circles[circle1Index].getxVelocity() * deltaTime);
        circles[circle1Index].addyVelocity(forceY/*/ circles[circle1Index].getMass()*/ - damping * circles[circle1Index].getyVelocity() * deltaTime);
        circles[circle2Index].addxVelocity(- forceX/*/ circles[circle2Index].getMass()*/ - damping * circles[circle2Index].getxVelocity() * deltaTime);
        circles[circle2Index].addyVelocity(- forceY/*/ circles[circle2Index].getMass()*/ - damping * circles[circle2Index].getyVelocity() * deltaTime);


    }

    void UpdateAll(float deltaTime) {
        for (Circle& circle : circles)
            circle.updatePosition(deltaTime);
    }
    void CheckCollisionsAndResolve(float wallxPosition, float wallxVelocity) {
        for (int i = 0; i < circles.size(); i++) {
            circles[i].ResolveCircleWallCollision(wallxPosition, screenHeight, wallxPosition, wallxVelocity);
            for (int j = i + 1; j < circles.size(); j++) {
                if (circles[i].CheckCircleCircleCollision(circles[j]))
                    circles[i].ResolveCircleCircleCollision(circles[j]);
            }
        }
    }



    void DrawAll(sf::RenderWindow& window) {
        for (Circle& circle : circles)
            circle.Draw(window);
    }

    vector<std::string> getStats() {
        velocityDistribution = vector<float>(circles.size());

        vector < std:: string > stats(3);
        float particleEnergy,
            internalEnergy = 0;
        int circle_index = 0;
        stats[0] = (std::to_string(circles.size()));

        for (Circle& circ : circles) {
            particleEnergy = circ.getxVelocity() * circ.getxVelocity() + circ.getyVelocity() * circ.getyVelocity();
            internalEnergy += particleEnergy;
            velocityDistribution[circle_index] = sqrt(particleEnergy);
            circle_index++;
        }
        stats[1] = std::to_string((internalEnergy));
        stats[2] = std::to_string(velocityDistribution.back());
        return stats;
    }

    void makeVelocityHistogram(sf::RenderWindow& window, int numbins, float expectedMaxVelocity) {
        int velocity_index = 0;
        vector<int> binoccupancy(numbins, 0);
        float binwidth = expectedMaxVelocity / numbins;

        for (float value : velocityDistribution) {
            int binIndex = value/binwidth;
            if (value > expectedMaxVelocity) binIndex = numbins-1;
            binoccupancy[binIndex] ++;
        }
        float rectangleWidth = 5;
        for (int i = 0; i < numbins; i++) {
            sf::RectangleShape rectangle(sf::Vector2f(rectangleWidth, binoccupancy[i] * 2));
            rectangle.setFillColor(sf::Color::White);
            rectangle.setPosition(sf::Vector2f(1015 + i * rectangleWidth, screenHeight - binoccupancy[i] * 2));
            window.draw(rectangle);
        }


    }
};