#include "Mesh.h"


Mesh::Mesh(int vertexCount, int polygonCount, std::vector<Point> points, std::vector<std::vector<int>> polygons) :
	vertexCount{vertexCount},
	polygonCount{polygonCount},
	vertices(std::move(points)),
	polygons(std::move(polygons))
{

}

void Mesh::Translate(const int xAmount, const int yAmount, const int zAmount)
{
	for (auto& vertex : vertices)
	{
		vertex.x += xAmount;
		vertex.y += yAmount;
		vertex.z += zAmount;
	}
}
