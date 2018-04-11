#include "Mesh.h"
#include "GraphicsMath.h"
#include <tuple>
#include <iostream>
#include <string>


Mesh::Mesh()
{
	vertex_count = 0;
	polygon_count = 0;
	vertices = std::vector<Point>();
	polygons = std::vector< std::vector<int>>();

	x_ = 0;
	y_ = 0;
	z_ = 0;

	// Initial Scale
	scaleFactor_ = 1.0;

	// Initial Rotation
	rotationX_ = 0;
	rotationY_ = 0;
	rotationZ_ = 0;
}

Mesh::Mesh(int vertexCount, int polygonCount, std::vector<Point> points, std::vector<std::vector<int>> polygons) :
	vertex_count{vertexCount},
	polygon_count{polygonCount},
	vertices(std::move(points)),
	polygons(std::move(polygons))
{
	// Initial Translation
	x_ = 0;
	y_ = 0;
	z_ = 0;
	
	// Initial Scale
	scaleFactor_ = 1.0;

	// Initial Rotation
	rotationX_ = 0;
	rotationY_ = 0; 
	rotationZ_ = 0;
}

void Mesh::Translate(const int xAmount, const int yAmount, const int zAmount)
{
	x_ += xAmount;
	y_ += yAmount;
	z_ += zAmount;
}

auto Mesh::Rotate(const float xAmount, const float yAmount, const float zAmount) -> void
{
	rotationX_ += xAmount;
	rotationY_ += yAmount;
	rotationZ_ += zAmount;
}

auto Mesh::Scale(const double scaleFactor) -> void
{
	Mesh::scaleFactor_ += scaleFactor;
}

auto Mesh::PrintStatus() const -> void
{
	std::cout << "Position:" << " x: " << x_ << " y: " << y_ << " z: " << z_ << std::endl;
//	std::cout << "Scale: " << scaleFactor << std::endl;
	std::cout << "Position:" << " Rotation:" << " x: " << rotationX_ << " y: " << rotationY_ << " z: " << rotationZ_ << std::endl;
}

auto Mesh::ResetRotation() -> void
{
	rotationX_ = 0;
	rotationY_ = 0;
	rotationZ_ = 0;
}

auto Mesh::MutableRotate(float xAmount, float yAmount, float zAmount) -> void
{
	for (auto& vertex : vertices)
	{
		vertex = GraphicsMath::RotatePoint(vertex, xAmount, yAmount, zAmount);
	}
}

auto Mesh::MutableScale(const float scaleFactor) -> void
{
	for (auto& vertex : vertices)
	{
		vertex.x *= scaleFactor;
		vertex.y *= scaleFactor;
		vertex.z *= scaleFactor;
	}
}

auto Mesh::TransformVertices() const -> std::vector<Point>
{
	auto transformedVertices = std::vector<Point>();

	// Scale Transform
	for (const auto& vertex : vertices)
	{
		transformedVertices.emplace_back(
			vertex.x * scaleFactor_,
			vertex.y * scaleFactor_,
			vertex.z * scaleFactor_,
			vertex.colour
		);
	}

	// Rotation Transforms
	for (auto& vertex : transformedVertices)
	{
		vertex = GraphicsMath::RotatePoint(vertex, rotationX_, rotationY_, rotationZ_);
	}

	// Position Transforms
	for (auto& vertex : transformedVertices)
	{
		vertex.x += x_;
		vertex.y += y_;
		vertex.z += z_;
	}

	return transformedVertices;
}
