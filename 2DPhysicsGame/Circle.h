#pragma once
#include <SFML/Graphics.hpp>
#include "Shape.h"

class Circle : public Shape{
public:
	//int index;
	float radius;
	//Point position;	
	//Point velocity;
	//Point acceleration;
	//float mass;
	//sf::Color color;

	Point oldAcceleration = Point(0, 0);
	/// <summary>
	/// 
	/// </summary>
	/// <param name="radius"></param>
	/// <param name="xPosition"></param>
	/// <param name="yPosition"></param>
	/// <param name="xVelocity"></param>
	/// <param name="yVelocity"></param>
	/// <param name="color"></param>
	Circle(int index, 
		float radius, 
		Point position, 
		Point velocity,
		float mass = 1,
		sf::Color color = sf::Color::White)
		: 
		Shape(position, velocity, index, mass, color), 
		radius(radius)
	{}



	// Gets
	float getRadius() {
		return radius;
	}

	// Get distance between this circle and otherCircle
	float getDistance(Circle otherCircle) {
		float distanceX = otherCircle.position.x - position.x;
		float distanceY = otherCircle.position.y - position.y;
		return std::sqrt(distanceX * distanceX + distanceY * distanceY);
	}

	// Handle Collisions
	bool CheckCircleCircleCollision(Circle& otherCircle) {
		float distanceX = otherCircle.position.x - position.x;
		float distanceY = otherCircle.position.y - position.y;
		float distanceSquared = distanceX * distanceX + distanceY * distanceY;

		float sumOfRadii = radius + otherCircle.getRadius();
		float sumOfRadiiSquared = sumOfRadii * sumOfRadii;

		return distanceSquared <= sumOfRadiiSquared;
	}

	// first, checks displaces circles if they are overlapping by half of the the overlap for each. Then, assuming equal mass, it resolves the collision with momentum and energy conservation.
	void ResolveCircleCircleCollision(Circle& othercircle) {
		//first, move circles out of overlap:
		float distanceX = othercircle.position.x - position.x;
		float distanceY = othercircle.position.y - position.y;
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

		float relativeVelocityX = othercircle.velocity.x - velocity.x;
		float relativeVelocityY = othercircle.velocity.y - velocity.y;

		float dotProduct = relativeVelocityX * normalX + relativeVelocityY * normalY;

		float impulse = (2.0f * dotProduct) / ((*this).getMass() + othercircle.getMass());

		float vx = impulse * othercircle.getMass() * normalX;
		float vy =impulse * othercircle.getMass() * normalY;
		Point dv(vx, vy);
		velocity += dv;
		othercircle.velocity += dv * (-1);

	}
	void ResolveWallCollision(float screenWidth, float screenHeight/*, float wallxVelocity*//*, bool& contact*/) override {
		if (position.x < radius) {
			position.x = radius;
			flipxVelocity();
		}
		else if (position.x + radius > screenWidth) {
			position.x = (screenWidth - radius); // something looks wrong here 
			(*this).flipxVelocity();
			//(*this).addxVelocity(wallxVelocity);
		}


		if (position.y <= radius) {
			(*this).flipyVelocity();
			position.y = (radius);
		}
		else if (position.y + radius > screenHeight) {
			(*this).flipyVelocity();
			position.y = (screenHeight - radius);
			//contact = true;
		}
	}

	//void circleFriction(float screenHeight) {
	//	float range = 0.1;
	//	float frictionCoefficient = 0.70; // fraction of speed lost for being in contact with the ground
	//	if (position.y > screenHeight - (radius + range)) {
	//		(*this).addxVelocity(-velocity.y * frictionCoefficient);
	//	}
	//}

	void Draw(sf::RenderWindow& window) {
		sf::CircleShape circleimage(radius);
		circleimage.setFillColor(color);
		circleimage.setPosition(sf::Vector2f(position.x - radius, position.y - radius));
		window.draw(circleimage);
	}
};