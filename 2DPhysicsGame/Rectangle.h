#pragma once
#include <SFML/Graphics.hpp>


class Rectangle {
private:
	int index;
	float rectangleHeight;
	float rectangleWidth;
	float xPosition;
	float yPosition;
	float xVelocity;
	float yVelocity;
	float xAcceleration = 0;
	float yAcceleration = 0;
	float mass;
	float angle;
	float angularVelocity;
	float angularAcceleration;
	sf::Color color;
	sf::RectangleShape rectangleImage;
public:
	float oldXAcceleration = 0;
	float oldYAcceleration = 0;
	Rectangle(float rectangleHeight, float rectangleWidth, float xPosition, float yPosition, float xVelocity, float yVelocity,
		float angle, float angularVelocity, float angularAcceleration, int index, float mass = 1, sf::Color color = sf::Color::White)
		: 
		rectangleHeight(rectangleHeight), rectangleWidth(rectangleWidth), index(index), xPosition(xPosition), yPosition(yPosition), xVelocity(xVelocity), yVelocity(yVelocity), angle(angle),
		angularVelocity(angularVelocity), angularAcceleration(angularAcceleration), mass(mass), color(color) 
	{

		rectangleImage = sf::RectangleShape(sf::Vector2f(rectangleWidth, rectangleHeight));
		rectangleImage.setFillColor(color);

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

	void addAngularVelocity(float omegaZ) {
		angularVelocity += omegaZ;
	}
	void addAngularAcceleration(float alphaZ) {
		angularAcceleration += alphaZ;
	}

	// Updates position given an initial position, velocity and acceleration using the Velocity Verlet integrator:
	void updatePostionVerlet(const float deltaTime) {
		float oldXPosition = xPosition;
		float oldYPosition = yPosition;
		float oldXVelocity = xVelocity;
		float oldYVelocity = yVelocity;
		float oldXAcceleration = xAcceleration;
		float oldYAcceleration = yAcceleration;
		xPosition += oldXVelocity * deltaTime + oldXAcceleration * deltaTime * deltaTime;
		yPosition += oldYVelocity * deltaTime + oldYAcceleration * deltaTime * deltaTime;

		// update accelerations

		xVelocity += (oldXAcceleration + xAcceleration) / 2 * deltaTime;
		yVelocity += (oldYAcceleration + yAcceleration) / 2 * deltaTime;

		oldXAcceleration = xAcceleration;
		oldYAcceleration = yAcceleration;
		xAcceleration = 0;
		yAcceleration = 0;
	}

	void Draw(sf::RenderWindow& window) {
		rectangleImage.setPosition(sf::Vector2f(xPosition, yPosition));
		rectangleImage.setRotation(angle);
		window.draw(rectangleImage);
	}
};