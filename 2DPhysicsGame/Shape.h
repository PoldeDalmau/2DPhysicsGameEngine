#pragma once
#include <SFML/Graphics.hpp>


class Shape {
private:


public:
	int index;
	float xPosition;
	float yPosition;
	//Point position;
	float xVelocity;
	float yVelocity;
	float xAcceleration = 0;
	float yAcceleration = 0;
	float mass;
	sf::Color color;
	float oldXAcceleration = 0;
	float oldYAcceleration = 0;

	Shape(
		float xPosition, 
		float yPosition, 
		float xVelocity, 
		float yVelocity,
		int index, 
		float mass = 1, 
		sf::Color color = sf::Color::White)
		:
		index(index),
		xPosition(xPosition),
		yPosition(yPosition),
		xVelocity(xVelocity),
		yVelocity(yVelocity), 
		mass(mass), 
		color(color) 
	{
	}

	// Gets
	int getIndex() {
		return index;
	}
	float getxPosition() {
		return xPosition;
	}
	float getyPosition() {
		return yPosition;
	}

	float getxVelocity() {
		return xVelocity;
	}
	float getyVelocity() {
		return yVelocity;
	}

	float getMass() {
		return mass;
	}

	// Updates position given a velocity using Euler integration:
	void updatePositionEuler(const float deltaTime) {
		xPosition += xVelocity * deltaTime;
		yPosition += yVelocity * deltaTime;
	}


	// Updates position given an initial position, velocity and acceleration using the Velocity Verlet integrator:
	// pending: not actually using the correct method
	// Acceleration is calculated from old position -> should be new position
	void updatePostionVerlet(const float deltaTime) {
		float oldXPosition = xPosition;
		float oldYPosition = yPosition;
		float oldXVelocity = xVelocity;
		float oldYVelocity = yVelocity;
		xPosition += oldXVelocity * deltaTime + oldXAcceleration * deltaTime * deltaTime;
		yPosition += oldYVelocity * deltaTime + oldYAcceleration * deltaTime * deltaTime;


		xVelocity += (oldXAcceleration + xAcceleration) / 2 * deltaTime;
		yVelocity += (oldYAcceleration + yAcceleration) / 2 * deltaTime;

		// update/ reset accelerations
		oldXAcceleration = xAcceleration;
		oldYAcceleration = yAcceleration;
		xAcceleration = 0;
		yAcceleration = 0;
	}

	// Sets
	void setX(float x) {
		xPosition = x;
	}
	void setY(float y) {
		yPosition = y;
	}
	void addxPosition(float x) {
		xPosition += x;
	}
	void addyPosition(float y) {
		yPosition += y;
	}



	void addxVelocity(float vx) {
		xVelocity += vx;
	}
	void addyVelocity(float vy) {
		yVelocity += vy;
	}
	void flipxVelocity() {
		xVelocity *= -1;
	}
	void flipyVelocity() {
		yVelocity *= -1;
	}
	void addxAcceleration(float ax) {
		xAcceleration += ax;
	}
	void addyAcceleration(float ay) {
		yAcceleration += ay;
	}

	// Handle Collisions
	virtual void ResolveWallCollision(float screenWidth, float screenHeight) {};


	// Draw
	virtual void Draw(sf::RenderWindow& window) {};
};


struct Point {
	float x;
	float y;
	Point() {
	}
	Point(float X, float Y) {
		x = X;
		y = Y;
	}

	Point operator + (const Point& other) {
		Point result;
		result.x = this->x + other.x;
		result.y = this->y + other.y;
		return result;
	}

	Point operator - (const Point& other) {
		Point result;
		result.x = this->x - other.x;
		result.y = this->y - other.y;
		return result;
	}

	Point operator * (const float scalar) {
		Point result;
		result.x = this->x * scalar;
		result.y = this->y * scalar;
		return result;
	}

	float dotProduct(Point other) {
		return (this->x * other.x + this->y * other.y);
	}
};