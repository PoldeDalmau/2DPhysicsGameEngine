#pragma once
#include <SFML/Graphics.hpp>
#include "Shape.h"

class Circle : public Shape{
public:
	float radius;
	Point oldAcceleration = Point(0, 0);
	const float floorRestitutionFactor;
	const float shapeRestitutionFactor;
	Circle(int index, 
		float radius, 
		Point position, 
		Point velocity,
		float floorRestitutionFactor,
		float shapeRestitutionFactor,
		float mass = 1,
		sf::Color color = sf::Color::White)
		: 
		Shape(position, velocity, index, mass, color), 
		floorRestitutionFactor(floorRestitutionFactor),
		shapeRestitutionFactor(shapeRestitutionFactor),
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
			position.x += (-displacementX * 0.5f);
			position.y += (-displacementY * 0.5f);
			othercircle.position.x += (displacementX * 0.5f);
			othercircle.position.y += (displacementY * 0.5f);

		}

		// then, handle the collision

		float normalX = distanceX / distanceMagnitude;
		float normalY = distanceY / distanceMagnitude;

		float relativeVelocityX = velocity.x - othercircle.velocity.x;
		float relativeVelocityY = velocity.y - othercircle.velocity.y;

		float dotProduct = relativeVelocityX * normalX + relativeVelocityY * normalY;

		float impulse = ( - (1.0f + shapeRestitutionFactor) * dotProduct) / (1/(*this).mass + 1/othercircle.mass);

		float vx = impulse * normalX;
		float vy = impulse * normalY;
		Point dv(vx, vy);
		this->velocity += dv * (1 / this->mass);
		othercircle.velocity += dv * (-1 / othercircle.mass);

	}
	void ResolveWallCollision(float screenWidth, float screenHeight/*, float wallxVelocity*//*, bool& contact*/) override {
		if (position.x < radius) {
			position.x = radius;
			flipxVelocity();
		}
		else if (position.x + radius > screenWidth) {
			position.x = (screenWidth - radius);
			(*this).flipxVelocity();
		}


		if (position.y <= radius) {
			(*this).flipyVelocity();
			position.y = (radius);
		}
		else if (position.y + radius > screenHeight) {
			this->velocity.y += -(1 + floorRestitutionFactor) * (this->velocity.y);
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