#pragma once
#include "Point.h"

class Face
{
public:
	Point a, b, c;
	Face(Point a, Point b, Point c);

	~Face() = default;
};

