#pragma once
#include <SFML/Graphics.hpp>
#include "Shape.h"


class Rectangle : public Shape {
public:
	float rectangleHeight;
	float rectangleWidth;
	float angle;
	float angularVelocity;
	float angularAcceleration;
	sf::RectangleShape rectangleImage;
	float halfdiag;
	float oldXAcceleration = 0;
	float oldYAcceleration = 0;
	int numAxes = 2; // rectangles have two axes
	Point vertexA, vertexB, vertexC, vertexD; // shape vertices
	Rectangle(
		float xPosition,
		float yPosition,
		float xVelocity,
		float yVelocity,
		int index,
		float mass,
		sf::Color color,
		float rectangleHeight, 
		float rectangleWidth, 
		float angle,
		float angularVelocity, 
		float angularAcceleration = 0)
		: 
		Shape(xPosition, yPosition, xVelocity, yVelocity, index, mass, color),
		rectangleHeight(rectangleHeight), 
		rectangleWidth(rectangleWidth), 
		angle(angle),
		angularVelocity(angularVelocity), 
		angularAcceleration(angularAcceleration)
	{
		halfdiag = 0.5 * sqrt(float(rectangleHeight) * rectangleHeight + rectangleWidth * rectangleWidth);
		rectangleImage = sf::RectangleShape(sf::Vector2f(rectangleWidth, rectangleHeight));
		rectangleImage.setFillColor(color);
		rectangleImage.setOrigin(sf::Vector2f(rectangleWidth/2,rectangleHeight/2));

		//A B
		//D C vertices with origin set in the middle of the shape without rotation
		vertexA = Point(-rectangleWidth/2, -rectangleHeight/2);
		vertexB = Point(rectangleWidth/2, -rectangleHeight/2);
		vertexC = Point(rectangleWidth/2, rectangleHeight/2);
		vertexD = Point(-rectangleWidth/2, rectangleHeight/2);
	}

	vector<Point> getVertices() {
		vector<Point> vertices(4);
		vertices[0] = rotate(vertexA) + Point(xPosition, yPosition);
		vertices[1] = rotate(vertexB) + Point(xPosition, yPosition);
		vertices[2] = rotate(vertexC) + Point(xPosition, yPosition);
		vertices[3] = rotate(vertexD) + Point(xPosition, yPosition);
		return vertices;
	}

	vector<Point> getAxes(Rectangle other) {
		vector<Point> axes(4);
		axes[0] = this->rotate(Point(1, 0));
		axes[1] = this->rotate(Point(0, 1));
		axes[2] = other.rotate(Point(1, 0));
		axes[3] = other.rotate(Point(0, 1));
		return axes;
	}

	void addAngularVelocity(float omegaZ) {
		angularVelocity += omegaZ;
	}

	void addAngle(float ang) {
		angle += ang;
	}

	void addAngularAcceleration(float alphaZ) {
		angularAcceleration += alphaZ;
	}

	void update(const float deltaTime) {
		updateAngleEuler(deltaTime);
		updatePositionEuler(deltaTime);
	}

	void updateAngleEuler(const float deltaTime) {
		angle += angularVelocity * deltaTime;
	}

	Point rotate(Point vertex) {
		float angleRad = angle * M_PI / 180; // convert to radians

		Point rotated;
		rotated.x = vertex.x * cos(angleRad) - vertex.y * sin(angleRad);
		rotated.y = vertex.x * sin(angleRad) + vertex.y * cos(angleRad);
		return rotated;
	}

	bool isNearRectangle(Rectangle other) {
		float pseudoRadius = halfdiag;
		float pseudoRadius2 = other.halfdiag;

		float distanceX = other.getxPosition() - xPosition;
		float distanceY = other.getyPosition() - yPosition;
		float distanceSquared = sqrt(distanceX * distanceX + distanceY * distanceY);

		if (distanceSquared <= pseudoRadius+ pseudoRadius2){
			return true;
		}
		else {
			rectangleImage.setFillColor(color);
			return false;
		}

	}
	bool isRectangleCollsion(Rectangle other) {
		// SAT 
		// find axes of each rectangle
		vector<Point> axes = getAxes(other);
		vector<Point> vertices1 = getVertices();
		vector<Point> vertices2 = other.getVertices();

		bool allAxesOverlap = false;
		float minProjection1;
		float maxProjection1;
		float minProjection2;
		float maxProjection2;

		for (Point& axis : axes) {
			minProjection1 = FLT_MAX;
			maxProjection1 = -FLT_MAX;
			minProjection2 = FLT_MAX;
			maxProjection2 = -FLT_MAX;
			//loop over points from first rectangle
			for (Point& vertex1 : vertices1) {
				if (axis.dotProduct(vertex1) < minProjection1)
					minProjection1 = axis.dotProduct(vertex1);
				if (axis.dotProduct(vertex1) > maxProjection1)
					maxProjection1 = axis.dotProduct(vertex1);
			}
			// Second rectangle
			for (Point& vertex2 : vertices2) {
				if (axis.dotProduct(vertex2) < minProjection2)
					minProjection2 = axis.dotProduct(vertex2);
				if (axis.dotProduct(vertex2) > maxProjection2)
					maxProjection2 = axis.dotProduct(vertex2);
			}
			// check overlap
			allAxesOverlap = false;
			if (minProjection1 < maxProjection2 && maxProjection1 > minProjection2) {
				allAxesOverlap = true;
			} else if (minProjection2 < maxProjection1 && maxProjection2 > minProjection1) {
				allAxesOverlap = true;
			}
			else {
				rectangleImage.setFillColor(sf::Color::Yellow);
				return false;
			}

			// MTD Minimum translation distance
		}
		rectangleImage.setFillColor(sf::Color::Red);
		return allAxesOverlap;
	}

	// preliminary collision test
	bool isNearWall(float screenWidth, float screenHeight) {
		if (xPosition - halfdiag <= 0 || xPosition + halfdiag >= screenWidth
			|| yPosition - halfdiag <= 0 || yPosition + halfdiag >= screenHeight)
			return true;
		else
			return false;
	}


	// handle collisions
	void ResolveWallCollision(float screenWidth, float screenHeight) override {
		//A B
		//D C
		vector<Point> vertices = getVertices();
		float minX = FLT_MAX;
		float minY = FLT_MAX;
		float maxX = - FLT_MAX;
		float maxY = - FLT_MAX;

		// find min and max x and y.
		for (Point& vertex : vertices) {
			if (vertex.x < minX)
				minX = vertex.x;
			if (vertex.y < minY)
				minY = vertex.y;
			if (vertex.x > maxX)
				maxX = vertex.x;
			if (vertex.y > maxY)
				maxY = vertex.y;
		}
		// left wall
		if (minX <= 0 && xVelocity<0) {
			flipxVelocity();
			addxPosition(-minX);
		} // right wall
		 if (maxX >= screenWidth && xVelocity > 0) {
			flipxVelocity();
			addxPosition(screenWidth - maxX);
		} // top wall
		else if (minY <= 0 && yVelocity < 0) {
			flipyVelocity();
			addyPosition(-minY);
		}// bottom wall
		else if (maxY >= screenHeight && yVelocity > 0) {
			flipyVelocity();
			addyPosition(screenHeight - maxY);
		}
	}

	// draw
	void Draw(sf::RenderWindow& window) override {
		rectangleImage.setPosition(sf::Vector2f(xPosition, yPosition));
		rectangleImage.setRotation(angle);
		window.draw(rectangleImage);
	}
};