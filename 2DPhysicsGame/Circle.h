#pragma once
#include <SFML/Graphics.hpp>

class Circle {
private:
	float radius;
	float xPosition;
	float yPosition;
	float xVelocity;
	float yVelocity;
	sf::Color color;

public:
	Circle(float radius, float xPosition, float yPosition, float xVelocity, float yVelocity, sf::Color color = sf::Color::White)
		: radius(radius), xPosition(xPosition), yPosition(yPosition), xVelocity(xVelocity), yVelocity(yVelocity), color(color) {
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
	
	// Handle Collisions
	bool CheckCircleCircleCollision(Circle& othercircle) {
		float distanceX = othercircle.getxPosition() - xPosition;
		float distanceY = othercircle.getyPosition() - yPosition;
		float distanceSquared = distanceX * distanceX + distanceY * distanceY;

		float sumOfRadii = radius + othercircle.getRadius();
		float sumOfRadiiSquared = sumOfRadii * sumOfRadii;

		return distanceSquared <= sumOfRadiiSquared;
	}

	// first, checks displaces circles if they are overlapping by half of the the overlap for each. Then, assuming equal mass, it resolves the collision with momentum and energy conservation.
	void ResolveCircleCircleCollision(Circle& othercircle) {
		//first, move circles out of overlap:
		float distanceX = othercircle.getxPosition() - xPosition;
		float distanceY = othercircle.getyPosition() - yPosition;
		float distanceMagnitude = std::sqrt(distanceX * distanceX + distanceY * distanceY);
		float overlap = (radius + othercircle.getRadius()) - distanceMagnitude;


		if (overlap > 0.0f) {
			// Calculate the displacement needed to move out of overlap
			float displacementX = (overlap / distanceMagnitude) * distanceX;
			float displacementY = (overlap / distanceMagnitude) * distanceY;

			// Move the circles to non-overlapping positions
			(*this).addxPosition(-displacementX * 0.5f);
			(*this).addyPosition(-displacementY * 0.5f);


			othercircle.addxPosition(displacementX * 0.5f);
			othercircle.addyPosition(displacementY * 0.5f);

		}

		// then, handle the collision assuming equal mass:

		float normalX = distanceX / distanceMagnitude;
		float normalY = distanceY / distanceMagnitude;

		float relativeVelocityX = othercircle.getxVelocity() - (*this).getxVelocity();
		float relativeVelocityY = othercircle.getyVelocity() - (*this).getyVelocity();

		float dotProduct = relativeVelocityX * normalX + relativeVelocityY * normalY;

		float impulse = (2.0f * dotProduct) / (1.0f + 1.0f); // Assuming equal masses

		(*this).addxVelocity(impulse * normalX);
		(*this).addyVelocity(impulse * normalY);

		othercircle.addxVelocity(-impulse * normalX);
		othercircle.addyVelocity(-impulse * normalY);
	}
	void ResolveCircleWallCollision(float screenWidth, float screenHeight) {
		if (xPosition < radius) {
			(*this).setX(radius);
			(*this).flipxVelocity();
		}
		else if (xPosition + radius > screenWidth) {
			(*this).setX(screenWidth - radius);
			(*this).flipxVelocity();
		}

		if (yPosition < radius) {
			(*this).flipyVelocity();
			(*this).setY(radius);
		}
		else if (yPosition + radius > screenHeight) {
			(*this).flipyVelocity();
			(*this).setY(screenHeight - radius);
		}
	}

	void Draw(sf::RenderWindow& window) {
		sf::CircleShape circleimage(radius);
		circleimage.setFillColor(color);
		circleimage.setPosition(sf::Vector2f(xPosition - radius, yPosition - radius));
		window.draw(circleimage);
	}
};