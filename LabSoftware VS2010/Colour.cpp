#include "Colour.h"
#include <cmath>

Colour::Colour(unsigned char r, unsigned char g, unsigned char b)
{
	Colour::r = r;
	Colour::g = g;
	Colour::b = b;
}

Colour Colour::Interpolate(Colour colour, int steps, int index) const
{
	unsigned char newR = Colour::r + (static_cast<double>(colour.r - Colour::r) / steps) * index;
	unsigned char newG = Colour::g + (static_cast<double>(colour.g - Colour::g) / steps) * index;
	unsigned char newB = Colour::b + (static_cast<double>(colour.b - Colour::b) / steps) * index;

	return {newR, newG, newB};
}
