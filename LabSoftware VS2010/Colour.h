#pragma once
class Colour
{
public:
	unsigned char r, g, b;
	Colour(const unsigned char r, const unsigned char g, const unsigned char b);

	Colour Interpolate(Colour colour, int steps, int index) const;
};

