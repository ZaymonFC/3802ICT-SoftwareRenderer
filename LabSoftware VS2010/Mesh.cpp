#include "Mesh.h"


Mesh::Mesh(int vertexCount, int polygonCount, std::vector<Point> points, std::vector<std::vector<int>> polygons) :
	vertexCount{vertexCount},
	polygonCount{polygonCount},
	vertices(std::move(points)),
	polygons(std::move(polygons))
{
}
