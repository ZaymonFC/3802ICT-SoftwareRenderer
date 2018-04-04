#pragma once
#include <vector>
#include "Point.h"
#include "Face.h"

class Mesh
{
public:
	int vertexCount;
	int polygonCount;
	std::vector<Point> vertices;
	std::vector<std::vector<int>> polygons;

	Mesh(int vertexCount, int polygonCount, std::vector<Point> points, std::vector<std::vector<int>> polygons);

	void Translate(int xAmount, int yAmount, int zAmount);

	~Mesh() = default;
};

