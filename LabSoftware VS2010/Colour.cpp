#include "Colour.h"

Colour::Colour()
{
	r = 255;
	g = 255;
	b = 255;
};

Colour::Colour(const unsigned char r, const unsigned char g, const unsigned char b) : r{r}, g{g}, b{b}
{
}

Colour::Colour(const Colour& other) : r{other.r}, g{other.g}, b{other.b}
{
}

Colour Colour::Interpolate(const Colour colour, const int steps, const int index) const
{
	const unsigned char newR = r + (static_cast<double>(colour.r - r) / steps) * index;
	const unsigned char newG = g + (static_cast<double>(colour.g - g) / steps) * index;
	const unsigned char newB = b + (static_cast<double>(colour.b - b) / steps) * index;

	return {newR, newG, newB};
}
