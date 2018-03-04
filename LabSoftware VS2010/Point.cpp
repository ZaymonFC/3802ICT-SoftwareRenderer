#include "Point.h"
#include <iostream>

Point::Point(const int x, const int y)
{
	this->x = x;
	this->y = y;

	std::cout << x << ' ' << y << std::endl;
}

