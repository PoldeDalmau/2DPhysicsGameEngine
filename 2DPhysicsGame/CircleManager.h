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

    void AddCircle(Circle circle){
        circles.push_back(circle);        
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
            float y = latticeConstant + 3*(numy * latticeConstant);
            float vx = 0;
            float vy = 0;
            //float vx = -100 + 5*numx;
            //float vy = 100 - 5*numx;
            AddCircle(Circle(radius, x, y, vx, vy));
        }
        // make one circle to hit all the circles;
        AddCircle(Circle(radius, ScreenHeight - radius, screenWidth / 2.0f + radius, 3, -2000, sf::Color::Red));
        velocityDistribution = vector<float>(circles.size());
    }

    void UpdateAll(float deltaTime) {
        for (Circle& circle : circles)
            circle.updatePosition(deltaTime);
    }
    void CheckCollisionsAndResolve() {
        for (int i = 0; i < circles.size(); i++) {
            circles[i].ResolveCircleWallCollision(screenWidth, screenHeight);
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
            rectangle.setPosition(sf::Vector2f(1010 + i * rectangleWidth, screenHeight - binoccupancy[i] * 2));
            window.draw(rectangle);
        }


    }
};