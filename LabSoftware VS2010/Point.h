#pragma once
#include "Colour.h"

class Point
{
public:
	int x, y;
	Colour colour;

	Point(int x, int y);
	Point(int x, int y, Colour colour);
};

