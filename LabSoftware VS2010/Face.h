#pragma once
#include "Point.h"

class Face
{
public:
	Point A, B, C;
	Face(Point a, Point b, Point c);

	~Face() = default;
};

