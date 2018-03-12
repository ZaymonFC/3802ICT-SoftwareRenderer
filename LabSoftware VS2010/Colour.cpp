#include "Colour.h"
#include <cmath>

Colour::Colour()
{
	r = 255;
	g = 255;
	b = 255;
};

Colour::Colour(const unsigned char r, const unsigned char g, const unsigned char b) : r{r}, g{g}, b{b}
{	
}

Colour::Colour(const Colour& other)
{
	r = other.r;
	g = other.g;
	b = other.g;
}

Colour Colour::Interpolate(const Colour colour, const int steps, const int index) const
{
	const unsigned char newR = Colour::r + (static_cast<double>(colour.r - Colour::r) / steps) * index;
	const unsigned char newG = Colour::g + (static_cast<double>(colour.g - Colour::g) / steps) * index;
	const unsigned char newB = Colour::b + (static_cast<double>(colour.b - Colour::b) / steps) * index;

	return {newR, newG, newB};
}
