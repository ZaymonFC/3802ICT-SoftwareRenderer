#include "MeshLoader.h"
#include "json.hpp"
#include "Mesh.h"
#include "Point.h"

#include <iostream>
#include <fstream>



Mesh MeshLoader::LoadMesh(std::string fileName)
{
	std::ifstream fileStream(fileName);

	if (!fileStream.good())
	{
		std::cout << "Mesh file: " + fileName + " not found" << std::endl;
		system("PAUSE");
		exit(0);
	}

	json jsonMesh;

	fileStream >> jsonMesh;
	fileStream.close();

	const auto vertexCount = jsonMesh["VertexCount"].get<int>();
	const auto polygonCount = jsonMesh["PolygonCount"].get<int>();
	auto vertexData = jsonMesh["Vertices"].get<std::vector<std::vector<float>>>();
	auto polygonData = jsonMesh["Polygons"].get<std::vector<std::vector<int>>>();

	auto vertices = std::vector<Point>();
	auto polygons = std::vector<std::vector<int>>();

	for (const auto& v : vertexData)
	{
		vertices.emplace_back(v[0], v[1], v[2], 
			Colour(
				static_cast<unsigned char>(v[3]),
				static_cast<unsigned char>(v[4]),
				static_cast<unsigned char>(v[5])
			)
		);
	}

	for (const auto& p : polygonData)
	{
		polygons.emplace_back(p);
	}


	return Mesh(vertexCount, polygonCount, vertices, polygons);
}

