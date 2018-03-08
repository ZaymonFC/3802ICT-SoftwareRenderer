#pragma once
class Colour
{
public:
	unsigned char r, g, b;
	Colour(unsigned char r, unsigned char g, unsigned char b);

	Colour Interpolate(Colour colour, int steps, int index) const;
};

