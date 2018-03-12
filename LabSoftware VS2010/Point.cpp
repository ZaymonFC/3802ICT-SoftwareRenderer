#include "Point.h"

Point::Point(const int x, const int y) : x{x}, y{y}
{
	colour = Colour(255, 255, 255);
}

Point::Point(const int x, const int y, const Colour colour) : x{x}, y{y}, colour{colour}
{
}

