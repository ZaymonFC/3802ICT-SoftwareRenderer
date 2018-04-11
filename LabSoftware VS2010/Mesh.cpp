#include "Mesh.h"
#include "GraphicsMath.h"
#include <tuple>
#include <iostream>
#include <string>
#include "MatrixMath.h"

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

	cached = false;
	cachedFaces = std::vector<Face>();
}

void Mesh::Translate(const float xAmount, const float yAmount, const float zAmount)
{
	x_ += xAmount;
	y_ += yAmount;
	z_ += zAmount;

	InvalidateCache();
}

auto Mesh::Rotate(const float xAmount, const float yAmount, const float zAmount) -> void
{
	rotationX_ += xAmount;
	rotationY_ += yAmount;
	rotationZ_ += zAmount;

	InvalidateCache();
}

auto Mesh::Scale(const double scaleFactor) -> void
{
	scaleFactor_ += scaleFactor;

	InvalidateCache();
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

	InvalidateCache();
}

auto Mesh::MutableRotate(float xAmount, float yAmount, float zAmount) -> void
{
	for (auto& vertex : vertices)
	{
		vertex = GraphicsMath::RotatePoint(vertex, xAmount, yAmount, zAmount);
	}

	InvalidateCache();
}

auto Mesh::MutableScale(const float scaleFactor) -> void
{
	for (auto& vertex : vertices)
	{
		vertex.x *= scaleFactor;
		vertex.y *= scaleFactor;
		vertex.z *= scaleFactor;
	}

	InvalidateCache();
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

//auto Mesh::TransformVertices() const->std::vector<Point>
//{
//	auto transformedVertices = std::vector<Point>();
//
//	MatrixMath::Mat4 positionMatrix = {
//		scaleFactor_, 0, 0, x_,
//		0, scaleFactor_, 0, y_,
//		0, 0, scaleFactor_, z_,
//		0, 0, 0, 1
//	};
//
//	MatrixMath::Mat4 rotationMatrixX = {
//		1, 0, 0, 0,
//		0, cos(rotationX_), -sin(rotationX_), 0,
//		0, sin(rotationX_), cos(rotationX_), 0,
//		0, 0, 0, 1
//	};
//	MatrixMath::Mat4 rotationMatrixY = {
//		cos(rotationY_), 0, sin(rotationY_), 0,
//		0, 1, 0, 0,
//		-sin(rotationY_), 0, cos(rotationY_), 0,
//		0, 0, 0, 1
//	};
//	MatrixMath::Mat4 rotationMatrixZ = {
//		cos(rotationZ_), -sin(rotationZ_), 0, 0,
//		sin(rotationZ_), cos(rotationZ_), 0, 0,
//		0, 0, 1, 0,
//		0, 0, 0, 1
//	};
//	const auto rotationMatrix = rotationMatrixX.Mat4Multiply(rotationMatrixY).Mat4Multiply(rotationMatrixZ);
//
//	for (auto vertex : vertices)
//	{
//		MatrixMath::TransformPoint(positionMatrix, vertex, 1);
//		MatrixMath::TransformPoint(rotationMatrix, vertex, 0);
//
//		transformedVertices.push_back(vertex);
//	}
//
//	return transformedVertices;
//}

auto Mesh::InvalidateCache() -> void
{
	cached = false;
}

