#pragma once

struct Point {
	float x;
	float y;

	// Default constructor
	Point() {
		x = 0;
		y = 0;
	}
	//~Point();
	Point(float X, float Y) {
		x = X;
		y = Y;
	}

	Point operator + (const Point& other) {
		Point result;
		result.x = this->x + other.x;
		result.y = this->y + other.y;
		return result;
	}

	Point operator - (const Point& other) {
		Point result;
		result.x = this->x - other.x;
		result.y = this->y - other.y;
		return result;
	}

	Point operator * (const float scalar) {
		Point result;
		result.x = this->x * scalar;
		result.y = this->y * scalar;
		return result;
	}

	void operator += (const Point other) {
		Point result;
		this->x += other.x;
		this->y += other.y;
	}

	float dotProduct(Point other) {
		return (this->x * other.x + this->y * other.y);
	}
};