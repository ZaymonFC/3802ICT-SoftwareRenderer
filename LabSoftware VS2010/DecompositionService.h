#pragma once
#include "Face.h"
#include <vector>
#include "GraphicsMath.h"

class DecompositionService
{
public:
	DecompositionService() = default;
	static std::vector<Face> DecomposePolygon(std::vector<Point> points);
};
