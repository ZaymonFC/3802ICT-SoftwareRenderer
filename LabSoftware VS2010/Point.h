#pragma once
#include "Colour.h"

/**
 * Container class for 2-3D Point information with an associated colour
 */
class Point
{
public:
	double x, y, z;
	Colour colour;

	Point();
	Point(double x, double y);
	Point(double x, double y, double z);
	Point(double x, double y, Colour colour);
	Point(double x, double y, double z, Colour colour);
	bool operator==(const Point & otherPoint) const;
};
