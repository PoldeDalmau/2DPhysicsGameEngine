#pragma once


class Shape {
private:
	int index;
	float xPosition;
	float yPosition;
	float xVelocity;
	float yVelocity;
	float xAcceleration = 0;
	float yAcceleration = 0;
	float mass;
};