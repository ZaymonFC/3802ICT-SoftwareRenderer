#pragma once
#include "Face.h"
#include <vector>

class DecompositionService
{
public:
	static bool ValidateTriangle(const std::vector<Point>& points, const Point& pOrigin, const Point& pPrev, const Point& pNext);
	static std::vector<Face> DecomposePolygon(std::vector<Point> points);
};
