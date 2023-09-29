#pragma once
//#include "Circle.h"
#include "CircleManager.h"
#include <vector>
#include <cassert>

#define _USE_MATH_DEFINES
#include <math.h>

class Car : public CircleManager{
private:
    float radius; // circle on wheel radius
    float Radius; // wheel radius
    float wheel_separation = 50;
//    vector<Circle> circles;
//    int numCircles;

public:
    Car(float screenWidth, float screenHeight, float deltaTime,
        sf::RenderWindow& window, float radius, float Radius) : CircleManager(screenWidth, screenHeight,
        deltaTime, window), radius(radius), Radius(Radius)/*, numCircles(0)*/
    {
        initializeWheels(100, 600);

    }

    void initializeWheels(float xPosition, float yPosition) {
        float relativeyPosition;
        float relativexPosition = 0;
        int numPoints = 10;

        float angle = 2 * M_PI / numPoints;
        float adjacentCircleDistance = 2 * Radius * sin(angle / 2);

        // first wheel
        AddCircle(Circle(radius * 0.1, xPosition, yPosition, 0, 0, numCircles, 1, sf::Color::Red));
        for (int i = 0; i < numPoints; i++) {
            relativexPosition = Radius * (cos(angle * i));
            relativeyPosition = Radius * (sin(angle * i));
            AddCircle(Circle(radius, xPosition + relativexPosition, yPosition + relativeyPosition, 0, 0, numCircles));
        }
        // second wheel
        xPosition += wheel_separation;

        AddCircle(Circle(radius * 0.1, xPosition, yPosition, 0, 0, numCircles, 1, sf::Color::Red));
        for (int i = 0; i < numPoints; i++) {
            relativexPosition = Radius * (cos(angle * i));
            relativeyPosition = Radius * (sin(angle * i));
            AddCircle(Circle(radius, xPosition + relativexPosition, yPosition + relativeyPosition, 0, 0, numCircles));
        }


    }

    void updateCar() {
        handleWheel();
        UpdateAll();
    }

    void handleWheel() {

        float internalDamping = 0.9;
        float k_radial = 50;
        float k_adjacent = 1000;
        int numPoints = 10;
        float angle = 2 * M_PI / numPoints;
        float adjacentCircleDistance = 2 * Radius * sin(angle / 2);

        // link between two wheels
        SpringLink(0, numPoints + 1, wheel_separation,10000, 0.99);



        // internal wheel forces
        for (int wheel = 0; wheel < 2; wheel++) {
            int index_shift = (numPoints + 1) * wheel;
            // adjacent links
            for (int i = 1 + index_shift; i < numPoints + index_shift; i++) {
                SpringLink(i, i + 1, adjacentCircleDistance, k_adjacent, internalDamping);
                SpringLink(i, 0 + index_shift, Radius, k_radial, internalDamping);
            }

            // diagonal links
            for (int i = 1 + index_shift; i < numPoints / 2 + index_shift; i++) {
                SpringLink(i, i + numPoints / 2 + 1, 2 * Radius * cos(angle / 2), k_radial, internalDamping);
                SpringLink(i, i + numPoints / 2 - 1, 2 * Radius * cos(angle / 2), k_radial, internalDamping);
            }
            // missing diagonal links
            SpringLink(numPoints / 2 + index_shift, numPoints - 1 + index_shift, 2 * Radius * cos(angle / 2), k_radial, internalDamping);
            SpringLink(numPoints / 2 + 1 + index_shift, numPoints + index_shift, 2 * Radius * cos(angle / 2), k_radial, internalDamping);



            // last circle with center
            SpringLink(10 + index_shift, 0 + index_shift, Radius, k_radial, internalDamping);
            // last circle with first circle
            SpringLink(10 + index_shift, 1 + index_shift, adjacentCircleDistance, k_adjacent, internalDamping);

        }

    }

    void turnWheel(int clockwise) {
        float foverm = clockwise * 1000 / Radius; // force causing rotation / mass / Radius of wheel
        float extraVelocity = - clockwise * 5; 
        int start = 1;
        int end = 10;
        int centerCircle = 0;
        for (int i = start; i < end + 1; i++) {
            float relativeX = circles[i].getxPosition() - circles[centerCircle].getxPosition();
            float relativeY = circles[i].getyPosition() - circles[centerCircle].getyPosition();
            circles[i].addxVelocity(foverm * relativeY * deltaTime + extraVelocity);
            circles[i].addyVelocity(-foverm * relativeX * deltaTime);
        }
    }
};