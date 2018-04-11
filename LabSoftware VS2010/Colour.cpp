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

Colour Colour::Interpolate(const Colour colour, const float steps, const float index) const
{
	const unsigned char newR = r + static_cast<unsigned char>((static_cast<float>(colour.r - r) / steps) * index);
	const unsigned char newG = g + static_cast<unsigned char>((static_cast<float>(colour.g - g) / steps) * index);
	const unsigned char newB = b + static_cast<unsigned char>((static_cast<float>(colour.b - b) / steps) * index);

	return {newR, newG, newB};
}

bool Colour::operator==(const Colour& otherColour) const
{
	return (r == otherColour.r && g == otherColour.g && b == otherColour.b);
}
