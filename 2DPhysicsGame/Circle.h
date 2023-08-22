#pragma once
#include <SFML/Graphics.hpp>

class Circle {
private:
	float radius;
	float xPosition;
	float yPosition;
	float xVelocity;
	float yVelocity;

public:
	Circle(float radius, float xPosition, float yPosition, float xVelocity, float yVelocity)
		: radius(radius), xPosition(xPosition), yPosition(yPosition), xVelocity(xVelocity), yVelocity(yVelocity) {
	}

	// Gets
	float getRadius() {
		return radius;
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

	// Updates position given a velocity:
	void updatePosition(float deltaTime) {
		xPosition += xVelocity * deltaTime;
		yPosition += yVelocity * deltaTime;
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
	


	// draw
	void Draw(sf::RenderWindow& window) {
		sf::CircleShape circle(radius);
		circle.setFillColor(sf::Color::White);
		circle.setPosition(sf::Vector2f(xPosition - radius, yPosition - radius));
		window.draw(circle);
	}
};