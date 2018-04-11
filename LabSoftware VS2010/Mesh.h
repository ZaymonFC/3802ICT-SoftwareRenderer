#pragma once
#include <vector>
#include "Point.h"

/**
* Container class for mesh and mesh operations such as translation and rotation
*/
class Mesh
{
	int x_;
	int y_;
	int z_;
	float rotationX_;
	float rotationY_;
	float rotationZ_;
	double scaleFactor_;
public:
	int vertex_count;
	int polygon_count;
	std::vector<Point> vertices;
	std::vector<std::vector<int>> polygons;

	Mesh();
	Mesh(int vertexCount, int polygonCount, std::vector<Point> points, std::vector<std::vector<int>> polygons);
	
	// Modifiers
	void Translate(int xAmount, int yAmount, int zAmount);
	auto Rotate(float xAmount, float yAmount, float zAmount) -> void;
	auto Scale(double scaleFactor) -> void;
	auto ResetRotation() -> void;

	// Mutable Modifiers
	auto MutableRotate(float xAmount, float yAmount, float zAmount) -> void;
	auto MutableScale(float scaleFactor) -> void;

	// Getters
	auto TransformVertices() const -> std::vector<Point>;

	// Printer
	auto PrintStatus() const -> void;
};

