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

	// Updates position given a velocity using Euler integration:
	void updatePositionEuler(const float deltaTime) {
		//xPosition += xVelocity * deltaTime;
		//yPosition += yVelocity * deltaTime;
		position += velocity * deltaTime;
	}


	// Updates position given an initial position, velocity and acceleration using the Velocity Verlet integrator:
	// pending: not actually using the correct method
	void updatePostionVerlet(const float deltaTime) {
		Point oldPos = position;
		Point oldVel = velocity;
		position += oldVel * deltaTime + oldAcceleration * deltaTime * deltaTime;

		velocity += (oldAcceleration + acceleration) * 0.5 * deltaTime;
		// update/ reset accelerations
		oldAcceleration = acceleration;
		acceleration = Point(0, 0);
	}

	void flipxVelocity() {
		//xVelocity *= -1;
		velocity.x *= -1;
	}
	void flipyVelocity() {
		//yVelocity *= -1;
		velocity.y *= -1;
	}

	// Gravity
	void Gravity(float g) {
		this->acceleration.y += g;
	}

	// Handle Collisions
	virtual void ResolveWallCollision(float screenWidth, float screenHeight) {};


	// Draw
	virtual void Draw(sf::RenderWindow& window) {};
};

