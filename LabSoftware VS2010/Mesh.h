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
	float rotationX;
	float rotationY;
	float rotationZ;
	double scaleFactor;
public:
	int vertexCount;
	int polygonCount;
	std::vector<Point> vertices;
	std::vector<std::vector<int>> polygons;

	Mesh(int vertexCount, int polygonCount, std::vector<Point> points, std::vector<std::vector<int>> polygons);

	void Translate(int xAmount, int yAmount, int zAmount);
	auto Rotate(float xAmount, float yAmount, float zAmount) -> void;
	auto Scale(double scaleFactor) -> void;
	auto TransformVertices() const -> std::vector<Point>;
	auto PrintStatus() const -> void;

};

