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
    float wheel_separation = 3.7 * Radius;
    int numPointsOnWheel = 20;
    bool canJump = true;
//    vector<Circle> circles;
//    int numCircles;

public:
    Car(float screenWidth, float screenHeight, float deltaTime,
        sf::RenderWindow& window, float radius, float Radius) : CircleManager(screenWidth, screenHeight,
        deltaTime, window), radius(radius), Radius(Radius)/*, numCircles(0)*/
    {
        initializeWheels(100, 200);
    }

    void initializeWheels(float xPosition, float yPosition) {
        float relativeyPosition;
        float relativexPosition = 0;

        float angle = 2 * M_PI / numPointsOnWheel;
        float adjacentCircleDistance = 2 * Radius * sin(angle / 2);

        // first wheel
        AddCircle(Circle(radius * 0.1, xPosition, yPosition, 0, 0, numCircles, 1, sf::Color::Red));
        for (int i = 0; i < numPointsOnWheel; i++) {
            relativexPosition = Radius * (cos(angle * i));
            relativeyPosition = Radius * (sin(angle * i));
            AddCircle(Circle(radius, xPosition + relativexPosition, yPosition + relativeyPosition, 0, 0, numCircles));
        }
        // second wheel
        xPosition += wheel_separation;

        AddCircle(Circle(radius * 0.1, xPosition, yPosition, 0, 0, numCircles, 1, sf::Color::Red));
        for (int i = 0; i < numPointsOnWheel; i++) {
            relativexPosition = Radius * (cos(angle * i));
            relativeyPosition = Radius * (sin(angle * i));
            AddCircle(Circle(radius, xPosition + relativexPosition, yPosition + relativeyPosition, 0, 0, numCircles));
        }

        // add chassis
        AddCircle(Circle(radius, xPosition - Radius/2, yPosition - 2 * Radius, 0, 0, numCircles));
        AddCircle(Circle(radius, (xPosition - wheel_separation) + Radius / 2, yPosition - 2 * Radius, 0, 0, numCircles));
    }

    void updateCar() {
        handleSprings();
        CheckCollisionsAndResolve(canJump);
        UpdateAll();
    }

    void handleSprings() {

        float internalDamping = 0.3;
        float k_radial = 200;
        float k_adjacent = 1000;
        float angle = 2 * M_PI / numPointsOnWheel;
        float adjacentCircleDistance = 2 * Radius * sin(angle / 2);
        float diagDist = 2 * Radius * cos(angle / 2);
        // link between two wheels
        SpringLink(0, numPointsOnWheel + 1, wheel_separation,10000, 0.99);
        // internal wheel forces
        for (int wheel = 0; wheel < 2; wheel++) {
            int index_shift = (numPointsOnWheel + 1) * wheel;
            // adjacent links
            for (int i = 1 + index_shift; i < numPointsOnWheel + index_shift; i++) {
                SpringLink(i, i + 1, adjacentCircleDistance, k_adjacent, internalDamping);
                SpringLink(i, 0 + index_shift, Radius, k_radial, internalDamping);
            }
            // diagonal links
            for (int i = 1 + index_shift; i < numPointsOnWheel / 2 + index_shift; i++) {
                SpringLink(i, i + numPointsOnWheel / 2 + 1, diagDist, k_radial, internalDamping);
                SpringLink(i, i + numPointsOnWheel / 2 - 1, diagDist, k_radial, internalDamping);
            }
            // missing diagonal links
            SpringLink(numPointsOnWheel / 2 + index_shift, numPointsOnWheel - 1 + index_shift, diagDist, k_radial, internalDamping);
            SpringLink(numPointsOnWheel / 2 + 1 + index_shift, numPointsOnWheel + index_shift, diagDist, k_radial, internalDamping);

            // last circle with center
            SpringLink(numPointsOnWheel + index_shift, 0 + index_shift, Radius, k_radial, internalDamping);
            // last circle with first circle
            SpringLink(numPointsOnWheel + index_shift, 1 + index_shift, adjacentCircleDistance, k_adjacent, internalDamping);
        }
        // chassis structure
        // chassis parts
        SpringLink(numPointsOnWheel * 2 + 2, numPointsOnWheel * 2 + 2 + 1, wheel_separation - Radius);
        // chassis with wheels
        float chassisWheelDist = Radius * sqrt(1/4 + 4); // sqrt(Radius/2^2 + 2 Radius^2)
        float chassisWheelDistDiag = sqrt(Radius*Radius*(1/4 + 4) + (wheel_separation - Radius/2)* (wheel_separation - Radius / 2)); // sqrt(Radius/2^2 + 2 Radius^2)
        SpringLink(0, numPointsOnWheel * 2 + 2 + 1, chassisWheelDist);
        SpringLink(numPointsOnWheel + 1, numPointsOnWheel * 2 + 2, chassisWheelDist);
        SpringLink(numPointsOnWheel + 1, numPointsOnWheel * 2 + 2 + 1, chassisWheelDistDiag);
        SpringLink(0, numPointsOnWheel * 2 + 2, chassisWheelDistDiag);

    }
    // Controls

    void turnWheel(int clockwise) {
        float foverm = clockwise * 5000 / Radius; // force causing rotation / mass / Radius of wheel
        float extraVelocity = - clockwise * 50; 
        int start = 1;
        int end = numPointsOnWheel;
        int centerCircle = 0;
        for (int i = start; i < end + 1; i++) {
            float relativeX = circles[i].getxPosition() - circles[centerCircle].getxPosition();
            float relativeY = circles[i].getyPosition() - circles[centerCircle].getyPosition();
            circles[i].addxVelocity(foverm * relativeY * deltaTime + extraVelocity);
            circles[i].addyVelocity(-foverm * relativeX * deltaTime);
        }
    }

    void jump() {
        if (canJump) {
            for (Circle& circle : circles)
                circle.addyVelocity(-175);
            // can only jump if touching the ground
            canJump = false;
        }
    }

};