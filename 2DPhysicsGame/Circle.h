#pragma once
#include <SFML/Graphics.hpp>

class Circle {
private:
	int index;
	float radius;
	float xPosition;
	float yPosition;
	float xVelocity;
	float yVelocity;
	float mass;
	sf::Color color;

public:
	/// <summary>
	/// 
	/// </summary>
	/// <param name="radius"></param>
	/// <param name="xPosition"></param>
	/// <param name="yPosition"></param>
	/// <param name="xVelocity"></param>
	/// <param name="yVelocity"></param>
	/// <param name="color"></param>
	Circle(float radius, float xPosition, float yPosition, float xVelocity, float yVelocity, int index, float mass = 1, sf::Color color = sf::Color::White)
		: index(index), radius(radius), xPosition(xPosition), yPosition(yPosition), xVelocity(xVelocity), yVelocity(yVelocity), mass(mass), color(color) {
	}



	// Gets
	int getIndex() {
		return this->index;
	}
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

	float getMass() {
		return mass;
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
	
	// Get distance between this circle and otherCircle
	float getDistance(Circle otherCircle) {
		float distanceX = otherCircle.getxPosition() - xPosition;
		float distanceY = otherCircle.getyPosition() - yPosition;
		return std::sqrt(distanceX * distanceX + distanceY * distanceY);
	}

	// Handle Collisions
	bool CheckCircleCircleCollision(Circle& otherCircle) {
		float distanceX = otherCircle.getxPosition() - xPosition;
		float distanceY = otherCircle.getyPosition() - yPosition;
		float distanceSquared = distanceX * distanceX + distanceY * distanceY;

		float sumOfRadii = radius + otherCircle.getRadius();
		float sumOfRadiiSquared = sumOfRadii * sumOfRadii;

		return distanceSquared <= sumOfRadiiSquared;
	}

	// first, checks displaces circles if they are overlapping by half of the the overlap for each. Then, assuming equal mass, it resolves the collision with momentum and energy conservation.
	void ResolveCircleCircleCollision(Circle& othercircle) {
		//first, move circles out of overlap:
		float distanceX = othercircle.getxPosition() - xPosition;
		float distanceY = othercircle.getyPosition() - yPosition;
		float distanceMagnitude = (*this).getDistance(othercircle);
		float overlap = (radius + othercircle.getRadius()) - distanceMagnitude;


		if (overlap > 0.0f) {
			// Calculate the displacement needed to move out of overlap
			float displacementX = (overlap / distanceMagnitude) * distanceX;
			float displacementY = (overlap / distanceMagnitude) * distanceY;

			// Move the circles to non-overlapping positions
			//if(getIndex() )

			//if (getIndex() > 9){ // hardcoded, change later
				addxPosition(-displacementX * 0.5f);
				addyPosition(-displacementY * 0.5f);
			//}
			//if (othercircle.getIndex() > 9) {
				othercircle.addxPosition(displacementX * 0.5f);
				othercircle.addyPosition(displacementY * 0.5f);
			//}

		}

		// then, handle the collision assuming equal mass:

		float normalX = distanceX / distanceMagnitude;
		float normalY = distanceY / distanceMagnitude;

		float relativeVelocityX = othercircle.getxVelocity() - (*this).getxVelocity();
		float relativeVelocityY = othercircle.getyVelocity() - (*this).getyVelocity();

		float dotProduct = relativeVelocityX * normalX + relativeVelocityY * normalY;

		float impulse = (2.0f * dotProduct) / ((*this).getMass() + othercircle.getMass()); // Assuming equal masses

		(*this).addxVelocity(impulse * othercircle.getMass() * normalX);
		(*this).addyVelocity(impulse * othercircle.getMass() * normalY);

		othercircle.addxVelocity(-impulse * (*this).getMass() * normalX);
		othercircle.addyVelocity(-impulse * (*this).getMass() * normalY);
	}
	void ResolveCircleWallCollision(float screenWidth, float screenHeight, float wallxPosition, float wallxVelocity) {
		if (xPosition < radius) {
			(*this).setX(radius);
			(*this).flipxVelocity();
		}
		else if (xPosition + radius > wallxPosition) {
			(*this).setX(screenWidth - radius);
			(*this).flipxVelocity();
			(*this).addxVelocity(wallxVelocity);
		}


		if (yPosition <= radius) {
			(*this).flipyVelocity();
			(*this).setY(radius);
		}
		else if (yPosition + radius > screenHeight) {
			(*this).flipyVelocity();
			(*this).setY(screenHeight - radius);
		}
	}

	void circleFriction(float screenHeight) {
		float range = 0.1;
		float frictionCoefficient = 0.7; // fraction of speed lost for being in contact with the ground
		if (yPosition > screenHeight - (radius + range)) {
			(*this).addxVelocity(-getxVelocity() * frictionCoefficient);
		}
	}

	void Draw(sf::RenderWindow& window) {
		sf::CircleShape circleimage(radius);
		circleimage.setFillColor(color);
		circleimage.setPosition(sf::Vector2f(xPosition - radius, yPosition - radius));
		window.draw(circleimage);
	}
};