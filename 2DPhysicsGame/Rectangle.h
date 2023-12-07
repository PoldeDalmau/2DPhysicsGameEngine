#pragma once
#include <SFML/Graphics.hpp>
#include "Shape.h"


class Rectangle : public Shape {
public:
	float rectangleHeight;
	float rectangleWidth;
	float angle;
	float angularVelocity;
	sf::RectangleShape rectangleImage;
	float halfdiag;
	//int numAxes = 2; // rectangles have two axes
	Point vertexA, vertexB, vertexC, vertexD; // shape vertices
	float angularAcceleration = 0;
	Rectangle(
		Point position,
		Point velocity,
		int index,
		float mass,
		sf::Color color,
		float rectangleHeight, 
		float rectangleWidth, 
		float angle,
		float angularVelocity)
		: 
		Shape(position, velocity, index, mass, color),
		rectangleHeight(rectangleHeight), 
		rectangleWidth(rectangleWidth), 
		angle(angle),
		angularVelocity(angularVelocity)
	{
		halfdiag = 0.5 * sqrt(rectangleHeight * rectangleHeight + rectangleWidth * rectangleWidth);
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
		vertices[0] = rotate(vertexA) + position;
		vertices[1] = rotate(vertexB) + position;
		vertices[2] = rotate(vertexC) + position;
		vertices[3] = rotate(vertexD) + position;
		return vertices;
	}

	vector<Point> getAxes() {
		vector<Point> axes(2);
		axes[0] = this->rotate(Point(1, 0));
		axes[1] = this->rotate(Point(0, 1));
		//axes[2] = other.rotate(Point(1, 0));
		//axes[3] = other.rotate(Point(0, 1));
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

	bool isNearCircle(Circle other) {
		float pseudoRadius = halfdiag;
		float pseudoRadius2 = other.radius;
		Point separationVector = other.position - this->position;
		float distanceSquared = sqrt(separationVector.x * separationVector.x + separationVector.y * separationVector.y);

		if (distanceSquared <= pseudoRadius + pseudoRadius2) {
			return true;
		}
		else {
			rectangleImage.setFillColor(color);
			return false;
		}
	}

	bool isNearRectangle(Rectangle other) {
		float pseudoRadius = halfdiag;
		float pseudoRadius2 = other.halfdiag;

		Point separationVector = other.position - this->position;
		float distanceSquared = sqrt(separationVector.x * separationVector.x + separationVector.y * separationVector.y);

		if (distanceSquared <= pseudoRadius+ pseudoRadius2){
			return true;
		}
		else {
			rectangleImage.setFillColor(color);
			return false;
		}

	}

	void isCircleCollsion(Circle& other) {
		// SAT 
		// find axes of each rectangle
		vector<Point> axes = this->getAxes();
		vector<Point> vertices1 = getVertices();

		Point mtv; // minimum translation vector
		float penetrationDepth = FLT_MAX;

		bool allAxesOverlap = false;
		float minProjection1;
		float maxProjection1;
		float minProjection2;
		float maxProjection2;

		for (Point& axis : axes) {
			minProjection1 = FLT_MAX;
			maxProjection1 = -FLT_MAX;
			minProjection2 = axis.dotProduct(other.position) - other.radius;
			maxProjection2 = axis.dotProduct(other.position) + other.radius;
			//loop over points from first rectangle
			for (Point& vertex1 : vertices1) {
				if (axis.dotProduct(vertex1) < minProjection1)
					minProjection1 = axis.dotProduct(vertex1);
				if (axis.dotProduct(vertex1) > maxProjection1)
					maxProjection1 = axis.dotProduct(vertex1);
			}
			// check overlap
			allAxesOverlap = false;
			if (minProjection1 < maxProjection2 && maxProjection1 > minProjection2) {
				allAxesOverlap = true;
				float tempDepth1 = abs(maxProjection1 - minProjection2);
				float tempDepth2 = abs(maxProjection2 - minProjection1);
				float tempDepth = tempDepth1 < tempDepth2 ? tempDepth1 : tempDepth2;

				if (penetrationDepth > tempDepth) {
					penetrationDepth = tempDepth;
					mtv = axis * penetrationDepth;
				}
			}
			else {
				rectangleImage.setFillColor(sf::Color::Yellow);
				return;
			}

		}
		rectangleImage.setFillColor(sf::Color::Red);
		// MTV Minimum Translation Vector
		// figure out the sign first (always away from the other object)
		float sign = -1;
		Point thisPos(position);
		Point otherPos(other.position);
		Point dist = otherPos - thisPos;
		if (mtv.dotProduct(dist) < 0)
			sign = 1;

		this->position += mtv * 0.5 * sign;
		other.position += mtv * 0.5 * (-sign);
	}


	void isRectangleCollsion(Rectangle &other) {
		// SAT 
		// find axes of each rectangle
		vector<Point> axes1 = this->getAxes();
		vector<Point> axes2 = other.getAxes();
		vector<Point> axes;
		for (Point& i : axes1) {
			axes.push_back(i);
		}
		for (Point& j : axes2) {
			axes.push_back(j);
		}
		
		
		vector<Point> vertices1 = getVertices();
		vector<Point> vertices2 = other.getVertices();
		Point mtv; // minimum translation vector
		float penetrationDepth = FLT_MAX;

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
				float tempDepth1 = abs(maxProjection1 - minProjection2);
				float tempDepth2 = abs(maxProjection2 - minProjection1);
				float tempDepth = tempDepth1 < tempDepth2 ? tempDepth1 : tempDepth2;

				if (penetrationDepth > tempDepth)	{
					penetrationDepth = tempDepth;
					mtv = axis * penetrationDepth;
				}
			}else {
				rectangleImage.setFillColor(sf::Color::Yellow);
				return;
			}

		}
		rectangleImage.setFillColor(sf::Color::Red);
		// MTV Minimum Translation Vector
		// figure out the sign first (always away from the other object)
		float sign = -1;
		Point thisPos(position);
		Point otherPos(other.position);
		Point dist = otherPos - thisPos;
		if (mtv.dotProduct(dist) < 0)
			sign = 1;

		this->position += mtv * 0.5 * sign;
		other.position += mtv * 0.5 * (-sign);
	}

	// preliminary collision test
	bool isNearWall(float screenWidth, float screenHeight) {
		if (position.x - halfdiag <= 0 || position.x + halfdiag >= screenWidth
			|| position.y - halfdiag <= 0 || position.y + halfdiag >= screenHeight)
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
		if (minX <= 0 && velocity.x<0) {
			flipxVelocity();
			addxPosition(-minX);
		} // right wall
		 if (maxX >= screenWidth && velocity.x > 0) {
			flipxVelocity();
			addxPosition(screenWidth - maxX);
		} // top wall
		else if (minY <= 0 && velocity.y < 0) {
			flipyVelocity();
			addyPosition(-minY);
		}// bottom wall
		else if (maxY >= screenHeight && velocity.y > 0) {
			flipyVelocity();
			addyPosition(screenHeight - maxY);
		}
	}

	// draw
	void Draw(sf::RenderWindow& window) override {
		rectangleImage.setPosition(sf::Vector2f(position.x, position.y));
		rectangleImage.setRotation(angle);
		window.draw(rectangleImage);
	}
};