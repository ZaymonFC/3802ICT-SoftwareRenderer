#pragma once
#include "Colour.h"

class Point
{
public:
	int x, y, z;
	Colour colour;

	Point(int x, int y);
	Point(int x, int y, int z);
	Point(int x, int y, Colour colour);
	Point(int x, int y, int z, Colour colour);
	bool operator==(const Point & otherPoint) const;
};
