#pragma once
#include <string>
#include "Mesh.h"
#include "json.hpp"

class MeshLoader
{
public:
	using json = nlohmann::json;

	static Mesh LoadMesh(std::string fileName);

	MeshLoader() = default;
	~MeshLoader() = default;
};

