#pragma once
#include <SFML/Graphics.hpp>
#include "Point.h"

class Shape {
public:
	Point position;
	Point velocity;
	Point acceleration = Point(0, 0);
	int index;
	float mass;
	sf::Color color;
	Point oldAcceleration = Point(0,0);

	Shape(
		Point position,
		Point velocity,
		int index, 
		float mass = 1, 
		sf::Color color = sf::Color::White)
		:
		position(position),
		velocity(velocity),
		mass(mass), 
		index(index),
		color(color) 
	{
	}

	// Gets
	int getIndex() {
		return index;
	}
	//float getxPosition() {
	//	return xPosition;
	//}
	//float getyPosition() {
	//	return yPosition;
	//}

	Point getPosition() {
		return position;
	}

	//float getxVelocity() {
	//	return xVelocity;
	//}
	//float getyVelocity() {
	//	return yVelocity;
	//}

	Point getVelocity() {
		return velocity;
	}

	float getMass() {
		return mass;
	}

	// Updates position given a velocity using Euler integration:
	void updatePositionEuler(const float deltaTime) {
		//xPosition += xVelocity * deltaTime;
		//yPosition += yVelocity * deltaTime;
		position += velocity * deltaTime;
	}


	// Updates position given an initial position, velocity and acceleration using the Velocity Verlet integrator:
	// pending: not actually using the correct method
	// Acceleration is calculated from old position -> should be new position
	void updatePostionVerlet(const float deltaTime) {
		Point oldPos = position;
		Point oldVel = velocity;
		//float oldXPosition = xPosition;
		//float oldYPosition = yPosition;
		//float oldXVelocity = xVelocity;
		//float oldYVelocity = yVelocity;
		//xPosition += oldXVelocity * deltaTime + oldXAcceleration * deltaTime * deltaTime;
		//yPosition += oldYVelocity * deltaTime + oldYAcceleration * deltaTime * deltaTime;
		position += oldVel * deltaTime + oldAcceleration * deltaTime * deltaTime;

		//xVelocity += (oldXAcceleration + xAcceleration) / 2 * deltaTime;
		//yVelocity += (oldYAcceleration + yAcceleration) / 2 * deltaTime;
		velocity += (oldAcceleration + acceleration) * 0.5 * deltaTime;
		// update/ reset accelerations
		oldAcceleration = acceleration;
		//oldXAcceleration = xAcceleration;
		//oldYAcceleration = yAcceleration;
		acceleration = Point(0, 0);
		//xAcceleration = 0;
		//yAcceleration = 0;
	}

	// Sets
	//void setX(float x) {
	//	xPosition = x;
	//}
	//void setY(float y) {
	//	yPosition = y;
	//}
	void addxPosition(float x) {
		position.x += x;
	}
	void addyPosition(float y) {
		position.y += y;
	}

	void addPosition(Point p) {
		this->position += p;
	}

	//void addxVelocity(float vx) {
	//	xVelocity += vx;
	//}
	//void addyVelocity(float vy) {
	//	yVelocity += vy;
	//}
	void flipxVelocity() {
		//xVelocity *= -1;
		velocity.x *= -1;
	}
	void flipyVelocity() {
		//yVelocity *= -1;
		velocity.y *= -1;
	}
	//void addxAcceleration(float ax) {
	//	xAcceleration += ax;
	//}
	//void addyAcceleration(float ay) {
	//	yAcceleration += ay;
	//}

	// Handle Collisions
	virtual void ResolveWallCollision(float screenWidth, float screenHeight) {};


	// Draw
	virtual void Draw(sf::RenderWindow& window) {};
};

