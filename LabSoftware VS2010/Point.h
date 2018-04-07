#pragma once
#include "Colour.h"

/**
 * Container class for 2-3D Point information with an associated colour
 */
class Point
{
public:
	float x, y, z;
	Colour colour;

	Point();
	Point(float x, float y);
	Point(float x, float y, float z);
	Point(float x, float y, Colour colour);
	Point(float x, float y, float z, Colour colour);
	bool operator==(const Point & otherPoint) const;
};
