#pragma once
#include <vector>
#include "Point.h"

/**
* Container class for mesh and mesh operations such as translation and rotation
*/
class Mesh
{
	int x;
	int y;
	int z;
public:
	int rotationX;
	int rotationY;
	int rotationZ;
	double scaleFactor;

	int vertexCount;
	int polygonCount;
	std::vector<Point> vertices;
	std::vector<std::vector<int>> polygons;

	Mesh(int vertexCount, int polygonCount, std::vector<Point> points, std::vector<std::vector<int>> polygons);

	void Translate(int xAmount, int yAmount, int zAmount);
	auto TransformVertices() const -> std::vector<Point>;

	auto Rotate(int xAmount, int yAmount, int zAmount) -> void;
	auto Scale(double scaleFactor) -> void;

};

