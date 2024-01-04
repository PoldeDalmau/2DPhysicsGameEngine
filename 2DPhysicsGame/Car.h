#pragma once
//#include "Circle.h"
#include "CircleManager.h"
#include "Point.h"
#include <vector>
#include <cassert>

#define _USE_MATH_DEFINES
#include <math.h>

class Car : public CircleManager{
private:
    float radius; // circle on wheel radius
    float Radius; // wheel radius
    float wheel_separation = 5 * Radius;
    int numPointsOnWheel = 20; // needs to be even number
    bool canJump = true;
//    vector<Circle> circles;
//    int numCircles;
    bool drawAll = true;
    float shapeRestitutionFactor = 1;

public:
    Car(
        float screenWidth,
        float screenHeight,
        const float deltaTime,
        sf::RenderWindow& window,
        float radius, 
        float Radius) : 
        CircleManager(screenWidth, screenHeight, deltaTime, window),
        radius(radius), 
        Radius(Radius)
        /*, numCircles(0)*/
    {
        initializeCar(100, 200);
    }

    void initializeCar(float xPosition, float yPosition) {
        float relativeyPosition;
        float relativexPosition = 0;

        float angle = 2 * M_PI / numPointsOnWheel;
        float adjacentCircleDistance = 2 * Radius * sin(angle / 2);

        // first wheel
        AddCircle(Circle(numCircles, radius * 0.1, Point(xPosition, yPosition),Point( 0, 0), shapeRestitutionFactor, shapeRestitutionFactor, 1, sf::Color::Red));
        for (int i = 0; i < numPointsOnWheel; i++) {
            relativexPosition = Radius * (cos(angle * i));
            relativeyPosition = Radius * (sin(angle * i));
            AddCircle(Circle(numCircles, radius, Point(xPosition + relativexPosition, yPosition + relativeyPosition), Point(0, 0), shapeRestitutionFactor, shapeRestitutionFactor));
        }
        // second wheel
        xPosition += wheel_separation;

        AddCircle(Circle(numCircles, radius * 0.1, Point(xPosition, yPosition), Point(0, 0), shapeRestitutionFactor, shapeRestitutionFactor, 1, sf::Color::Red));
        for (int i = 0; i < numPointsOnWheel; i++) {
            relativexPosition = Radius * (cos(angle * i));
            relativeyPosition = Radius * (sin(angle * i));
            AddCircle(Circle(numCircles, radius, Point(xPosition + relativexPosition, yPosition + relativeyPosition), Point(0, 0), shapeRestitutionFactor, shapeRestitutionFactor));
        }

        // add chassis
        AddCircle(Circle(numCircles, radius, Point(xPosition - Radius/2, yPosition - 2 * Radius), Point(0, 0), shapeRestitutionFactor, shapeRestitutionFactor));
        AddCircle(Circle(numCircles, radius, Point((xPosition - wheel_separation) + Radius / 2, yPosition - 2 * Radius), Point(0, 0), shapeRestitutionFactor, shapeRestitutionFactor));
    }

    void updateCar() {
        handleSprings();
        UpdateAll(canJump);
    }

    void handleSprings() {

        float internalDamping = 0.9;
        float k_radial = 200;
        float k_adjacent = 100;
        //float k_adjacent = 200;
        float k_chassis = 400;

        float angle = 2 * M_PI / numPointsOnWheel;
        float adjacentCircleDistance = 2 * Radius * sin(angle / 2);
        float diagDist = 2 * Radius * cos(angle / 2);
        // link between two wheels
        SpringLink(0, numPointsOnWheel + 1, wheel_separation, k_chassis, internalDamping, drawAll);
        // internal wheel forces
        for (int wheel = 0; wheel < 2; wheel++) {
            int index_shift = (numPointsOnWheel + 1) * wheel;
            // adjacent links
            for (int i = 1 + index_shift; i < numPointsOnWheel + index_shift; i++) {
                SpringLink(i, i + 1, adjacentCircleDistance, k_adjacent, internalDamping);
                SpringLink(i, 0 + index_shift, Radius, k_radial, internalDamping, drawAll);
            }
            // diagonal links
            for (int i = 1 + index_shift; i < numPointsOnWheel / 2 + index_shift; i++) {
                SpringLink(i, i + numPointsOnWheel / 2 + 1, diagDist, k_radial, internalDamping, drawAll);
                SpringLink(i, i + numPointsOnWheel / 2 - 1, diagDist, k_radial, internalDamping, drawAll);
            }
            // missing diagonal links
            SpringLink(numPointsOnWheel / 2 + index_shift, numPointsOnWheel - 1 + index_shift, diagDist, k_radial, internalDamping, drawAll);
            SpringLink(numPointsOnWheel / 2 + 1 + index_shift, numPointsOnWheel + index_shift, diagDist, k_radial, internalDamping, drawAll);

            // last circle with center
            SpringLink(numPointsOnWheel + index_shift, 0 + index_shift, Radius, k_radial, internalDamping, drawAll);
            // last circle with first circle
            SpringLink(numPointsOnWheel + index_shift, 1 + index_shift, adjacentCircleDistance, k_adjacent, internalDamping);
        }
        // chassis structure
        // chassis parts
        SpringLink(numPointsOnWheel * 2 + 2, numPointsOnWheel * 2 + 2 + 1, wheel_separation - Radius, k_adjacent, internalDamping);
         //chassis with wheels
        float chassisWheelDist = Radius * sqrt(1/4 + 4); // sqrt(Radius/2^2 + 2 Radius^2)
        float chassisWheelDistDiag = sqrt(Radius*Radius*(1/4 + 4) + (wheel_separation - Radius/2)* (wheel_separation - Radius / 2)); // sqrt(Radius/2^2 + 2 Radius^2)
        SpringLink(0, numPointsOnWheel * 2 + 2 + 1, chassisWheelDist, k_chassis, internalDamping);
        SpringLink(numPointsOnWheel + 1, numPointsOnWheel * 2 + 2, chassisWheelDist, k_chassis, internalDamping);
        SpringLink(numPointsOnWheel + 1, numPointsOnWheel * 2 + 2 + 1, chassisWheelDistDiag, k_chassis, internalDamping);
        SpringLink(0, numPointsOnWheel * 2 + 2, chassisWheelDistDiag, k_chassis, internalDamping);

    }


    // Controls

    void turnWheel(int clockwise) {
        float foverm = clockwise * 800 / Radius; // force causing rotation / mass / Radius of wheel
        //float extraVelocity = 0; 
        float extraVelocity = - clockwise * 2; 
        int start = 1;
        int end = numPointsOnWheel;
        for (int wheel = 0; wheel < 2; wheel++) {
            int index_shift = (numPointsOnWheel + 1) * wheel;
            for (int i = start + index_shift; i < end + 1 + index_shift; i++) {
                float relativeX = circles[i].position.x - circles[index_shift].position.x;
                float relativeY = circles[i].position.y - circles[index_shift].position.y;
                circles[i].velocity.x += (foverm * relativeY * deltaTime + extraVelocity);
                circles[i].velocity.y += (-foverm * relativeX * deltaTime);
            }
        }
    }

    void jump() {
        if (canJump) {
            for (Circle& circle : circles)
                circle.acceleration.y += (-10000);
            // can only jump if touching the ground
            //canJump = false;
        }
    }

    // Options
    void drawSprings() {
        drawAll = 1 - drawAll;
    }

};