#include "Point.h"

Point::Point()
{
	x = 0;
	y = 0;
	z = 0;
	colour = Colour(255, 255, 255);
}

Point::Point(const double x, const double y) : x{x}, y{y}
{
	z = 0;
	colour = Colour(255, 255, 255);
}

Point::Point(const double x, const double y, const double z) : x {x}, y{y}, z{z}
{
}

Point::Point(const double x, const double y, const Colour colour) : x{x}, y{y}, colour{colour}
{
	z = 0;
}

Point::Point(const double x, const double y, const double z, const Colour colour) : x{x}, y{y}, z{z}, colour{colour}
{
}

bool Point::operator==(const Point& otherPoint) const
{
	return (
		x == otherPoint.x &&
		y == otherPoint.y &&
		z == otherPoint.z &&
		colour == otherPoint.colour
	);
}
