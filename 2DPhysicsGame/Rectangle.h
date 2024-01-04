#pragma once
#include <SFML/Graphics.hpp>
#include "Shape.h"


class Rectangle : public Shape {
public:
	const float rectangleHeight;
	const float rectangleWidth;
	float angle;
	float angularVelocity;
	sf::RectangleShape rectangleImage;
	float halfdiag;
	//int numAxes = 2; // rectangles have two edges
	Point vertexA, vertexB, vertexC, vertexD; // shape vertices
	float angularAcceleration = 0;
	const float floorRestitutionFactor;
	const float shapeRestitutionFactor;
	float momentIntertia;
	Rectangle(
		Point position,
		Point velocity,
		int index,
		float mass,
		sf::Color color,
		float rectangleHeight, 
		float rectangleWidth, 
		float angle,
		float floorRestitutionFactor,
		float shapeRestitutionFactor,
		float angularVelocity)
		: 
		Shape(position, velocity, index, mass, color),
		rectangleHeight(rectangleHeight), 
		rectangleWidth(rectangleWidth), 
		angle(angle),
		floorRestitutionFactor(floorRestitutionFactor),
		shapeRestitutionFactor(shapeRestitutionFactor),
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

		momentIntertia = 1/12.0f * mass * (rectangleHeight * rectangleHeight + rectangleWidth * rectangleWidth);
	}

	vector<Point> getVertices() {
		vector<Point> vertices(4);
		vertices[0] = rotate(vertexA) + position;
		vertices[1] = rotate(vertexB) + position;
		vertices[2] = rotate(vertexC) + position;
		vertices[3] = rotate(vertexD) + position;
		return vertices;
	}

	vector<Point> getEdges() {
		vector<Point> edges(2);
		edges[0] = this->rotate(Point(1, 0));
		edges[1] = this->rotate(Point(0, 1));
		return edges;
	}

	void addAngularAcceleration(float alphaZ) {
		angularAcceleration += alphaZ;
	}

	void update(const float deltaTime) {
		updateAngleEuler(deltaTime);
		updatePostionVerlet(deltaTime);
		//updatePositionEuler(deltaTime);
	}

	void updateAngleEuler(const float deltaTime) {
		angle += angularVelocity * deltaTime;
	}

	Point rotate(Point vertex) {
		//float angleRad = angle * M_PI / 180; // convert to radians

		Point rotated;
		rotated.x = vertex.x * cos(angle) - vertex.y * sin(angle);
		rotated.y = vertex.x * sin(angle) + vertex.y * cos(angle);
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

	bool isCircleCollsion(Circle& other/*, sf::RenderWindow & window*/) {
		vector<Point> edges = this->getEdges();
		vector<Point> vertices1 = this->getVertices();

		Point mtv; // minimum translation vector
		float penetrationDepth = FLT_MAX;

		float minProjection1;
		float maxProjection1;
		float minProjection2;
		float maxProjection2;

		Point otherPos(other.position);

		
		// ckeck collision when circle collides with polygon side with SAT.

		Point tempaxis;
		for (Point& edge : edges) {
			minProjection1 = FLT_MAX;
			maxProjection1 = -FLT_MAX;
			minProjection2 = edge.dotProduct(otherPos) - other.radius;
			maxProjection2 = edge.dotProduct(otherPos) + other.radius;
			//loop over points from first rectangle
			for (Point& vertex1 : vertices1) {
				if (edge.dotProduct(vertex1) < minProjection1)
					minProjection1 = edge.dotProduct(vertex1);
				if (edge.dotProduct(vertex1) > maxProjection1)
					maxProjection1 = edge.dotProduct(vertex1);
			}
			// check overlap
			if (minProjection1 < maxProjection2 && maxProjection1 > minProjection2) {
			}
			else {
				//rectangleImage.setFillColor(sf::Color::Yellow);
				return false;
			}

		}
		//rectangleImage.setFillColor(sf::Color::Red);
		return true;
	}

	void resolveCircleCollision(Circle & other) {
		Point otherPos = other.position;
		Point thisPos = this->position;
		Point mtv;
		Point p, edge;
		getContactPointAndEdgeCircle(*this, other, p, edge);
		// figure out the sign first (always away from the other object)
		float sign = -1;
		// circle center to contact point:
		Point sep = other.position - p;
		float sepScalar = sqrt(sep.dotProduct(sep));

		Point dist = otherPos - thisPos;
		mtv = sep * (1 / sepScalar) * (other.radius - sepScalar)/* * 1.01f*/;
		if (mtv.dotProduct(dist) < 0)
			sign = 1;


		this->position += mtv * 0.5 * sign;
		other.position += mtv * 0.5 * (-sign);

		// resolve collision:
		Point collisionPoint = p;
		Point normal(-edge.y, edge.x); // points toward body A
		normal = normal * (1 / sqrt(edge.dotProduct(edge)));

		Point r_AP = collisionPoint - this->position;
		Point r_AP_perp(-r_AP.y, r_AP.x);

		Point r_BP = collisionPoint - other.position;
		Point v_AP = this->velocity + r_AP_perp * this->angularVelocity;
		Point v_BP = other.velocity;
		Point v_AB = v_AP - v_BP;
		// j: impulse
		float j = -(1 + shapeRestitutionFactor) * (v_AB).dotProduct(normal) / (1.0f / this->mass + 1.0f / other.mass + std::pow(r_AP_perp.dotProduct(normal), 2) / momentIntertia);

		this->velocity += normal * (j / mass);
		this->angularVelocity += r_AP_perp.dotProduct(normal) * j / momentIntertia;
		other.velocity += normal * (-j / other.mass);
		// Visualize contact point.
		sf::CircleShape X1;
		//X1.setFillColor(sf::Color::Green);
		//X1.setRadius(10.0f);
		//X1.setPosition(sf::Vector2f(p.x - X1.getRadius(), p.y - X1.getRadius()));
		//window.draw(X1);
	}

	void getContactPointAndEdgeCircle(Rectangle rect, Circle circ, Point& contact, Point& edge) { // Better to have collision handling functions in its own separate class
		vector<Point> vertices = rect.getVertices();
		float min_dist_squared = FLT_MAX;
		Point side, contactSide;
		Point relativeCirclePos;
		int closestSideIndex = 0;
		float l, lFinal;

		for (int i = 0; i < vertices.size(); i++) {
			side = vertices[(i + 1) % vertices.size()] - vertices[i];
			relativeCirclePos = circ.position - vertices[i];

			// point to line dist squared NOT SQUARED FOR NOW
			l = relativeCirclePos.dotProduct(side) / side.dotProduct(side);
			Point normalToCirc = (relativeCirclePos - side * side.dotProduct(relativeCirclePos)*(1 / side.dotProduct(side))) ;

			float new_min_dist_squared;
			if (l < 0) {
				new_min_dist_squared = relativeCirclePos.dotProduct(relativeCirclePos);
			}
			else if (l > 1) {
				new_min_dist_squared = (relativeCirclePos - side).dotProduct(relativeCirclePos - side);
			}
			else {
				new_min_dist_squared = (normalToCirc.dotProduct(normalToCirc));
			}
			

			if (new_min_dist_squared < min_dist_squared) {
				min_dist_squared = new_min_dist_squared; 
				closestSideIndex = i;
				contactSide = side;
				lFinal = l;
			}

		}
		// project circle center onto side and get contact point
		lFinal = lFinal < 0 ? 0 :
			(lFinal > 1 ? 1 : lFinal); // l is bound between 0 and 1
		contact = vertices[closestSideIndex] + contactSide * lFinal;
		edge = contactSide;
	}


	bool isRectangleCollsion(Rectangle &other) {
		// SAT 
		Point mtv; // minimum translation vector
		// find edges of each rectangle
		vector<Point> edges1 = this->getEdges();
		vector<Point> edges2 = other.getEdges();
		vector<Point> edges;
		for (Point& i : edges1) {
			edges.push_back(i);
		}
		for (Point& j : edges2) {
			edges.push_back(j);
		}
		
		
		vector<Point> vertices1 = getVertices();
		vector<Point> vertices2 = other.getVertices();
		float penetrationDepth = FLT_MAX;

		bool allAxesOverlap = false;
		float minProjection1;
		float maxProjection1;
		float minProjection2;
		float maxProjection2;
		Point finalEdge;
		for (Point& edge : edges) {
			minProjection1 = FLT_MAX;
			maxProjection1 = -FLT_MAX;
			minProjection2 = FLT_MAX;
			maxProjection2 = -FLT_MAX;
			//loop over points from first rectangle
			for (Point& vertex1 : vertices1) {
				if (edge.dotProduct(vertex1) < minProjection1)
					minProjection1 = edge.dotProduct(vertex1);
				if (edge.dotProduct(vertex1) > maxProjection1)
					maxProjection1 = edge.dotProduct(vertex1);
			}
			// Second rectangle
			for (Point& vertex2 : vertices2) {
				if (edge.dotProduct(vertex2) < minProjection2)
					minProjection2 = edge.dotProduct(vertex2);
				if (edge.dotProduct(vertex2) > maxProjection2)
					maxProjection2 = edge.dotProduct(vertex2);
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
					mtv = edge * penetrationDepth /** 1.01f*/; // increase separation for safety
				}
			} else {
				//rectangleImage.setFillColor(sf::Color::Yellow);
				return false;
			}
		}
		//rectangleImage.setFillColor(sf::Color::Red);
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


	// broadphase collision test
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
		vector<Point> p; // points of collision
		vector<Point> normals;
		// find min and max x and y. /FIX THIS AWFUL LOOP
		for (Point& vertex : vertices) {
			// left wall
			if (vertex.x < 0 /*&& velocity.x<0*/) {
				position.x -= vertex.x;
				normals.push_back(Point(1, 0));
				p.push_back(vertex);
				vertex.x = 0;
			} // right wall
			if (vertex.x > screenWidth/* && velocity.x > 0*/) {
				position.x += (screenWidth - vertex.x);
				vertex.x = screenWidth;
				normals.push_back(Point(-1, 0));
				p.push_back(vertex);
			} // top wall
			if (vertex.y < 0 /*&& velocity.y < 0*/) {
				position.y -= vertex.y;
				vertex.y = 0;
				normals.push_back(Point(0, 1));
				p.push_back(vertex);
			}// bottom wall
			if (vertex.y > screenHeight/* && velocity.y > 0*/) {
				position.y += (screenHeight - vertex.y);
				normals.push_back(Point(0, -1));
				vertex.y =screenHeight;
				p.push_back(vertex);
			}
		}

		Point pos = this->position;
		Point vel = this->velocity;
		float angVel = this->angularVelocity;
		for (int i = 0; i < p.size(); i++) {
			// resolve collision:
			Point r_AP = p[i] - pos;
			Point r_AP_perp(- r_AP.y, r_AP.x);
			Point v_AP = vel + r_AP_perp * angVel;
			// j: impulse
			float rotation = std::pow(r_AP_perp.dotProduct(normals[i]), 2) / momentIntertia;
			rotation = p.size() > 1 ? 0 : rotation;
			float j = -(1 + floorRestitutionFactor) * (v_AP).dotProduct(normals[i])/(1.0f /this->mass + rotation);
			j = j / float(p.size());
			this->velocity += normals[i] * (j / mass);
			this->angularVelocity += r_AP_perp.dotProduct(normals[i]) * j / momentIntertia;
		}
	}

	// draw
	void Draw(sf::RenderWindow& window) override {
		rectangleImage.setPosition(sf::Vector2f(position.x, position.y));
		rectangleImage.setRotation(angle * 180 / M_PI);
		window.draw(rectangleImage);
	}
};