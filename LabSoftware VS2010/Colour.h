#pragma once
class Colour
{
public:
	unsigned char r, g, b;
	Colour();
	Colour(unsigned char r, unsigned char g, unsigned char b);
	Colour(const Colour &other);
	Colour Interpolate(Colour colour, int steps, int index) const;
};
