#include "Colour.h"
#include <cmath>

Colour::Colour(const unsigned char r, const unsigned char g, const unsigned char b)
{
	Colour::r = r;
	Colour::g = g;
	Colour::b = b;
}

Colour Colour::Interpolate(const Colour colour, int steps, int index) const
{	
	unsigned char newR = Colour::r + ((Colour::r - colour.r) / steps) * index;
	unsigned char newG = Colour::g + ((Colour::g - colour.g) / steps) * index;
	unsigned char newB = Colour::b + ((Colour::b - colour.b) / steps) * index;

	return Colour { newR, newG, newB };
}
