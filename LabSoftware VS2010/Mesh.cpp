#include "Mesh.h"
#include "GraphicsMath.h"
#include <tuple>


Mesh::Mesh(int vertexCount, int polygonCount, std::vector<Point> points, std::vector<std::vector<int>> polygons) :
	vertexCount{vertexCount},
	polygonCount{polygonCount},
	vertices(std::move(points)),
	polygons(std::move(polygons))
{
	// Initial Translation
	x = 0;
	y = 0;
	z = 0;
	
	// Initial Scale
	scaleFactor = 1.0;

	// Initial Rotation
	rotationX = 0;
	rotationY = 0; 
	rotationZ = 0;
}

void Mesh::Translate(const int xAmount, const int yAmount, const int zAmount)
{
	x += xAmount;
	y += yAmount;
	z += zAmount;
}

auto Mesh::Rotate(const int xAmount, const int yAmount, const int zAmount) -> void
{
	rotationX += xAmount;
	rotationY += yAmount;
	rotationZ += zAmount;
}

auto Mesh::Scale(const double scaleFactor) -> void
{
	Mesh::scaleFactor += scaleFactor;
}

auto Mesh::TransformVertices() const -> std::vector<Point>
{
	auto transformedVertices = std::vector<Point>();

	// Scale Transform
	for (const auto& vertex : vertices)
	{
		transformedVertices.emplace_back(
			vertex.x * scaleFactor,
			vertex.y * scaleFactor,
			vertex.z * scaleFactor,
			vertex.colour
		);
	}

	// Rotation Transforms
	for (auto& vertex : transformedVertices)
	{
		vertex = GraphicsMath::RotatePoint(vertex, rotationX, rotationY, rotationZ);
	}

	// Position Transforms
	for (auto& vertex : transformedVertices)
	{
		vertex.x += x;
		vertex.y += y;
		vertex.z += z;
	}

	return transformedVertices;
}

