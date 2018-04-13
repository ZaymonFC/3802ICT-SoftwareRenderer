#pragma once
#include <string>
#include "Mesh.h"
#include "json.hpp"

class MeshLoader
{
public:
	using json = nlohmann::json;

	static auto LoadMesh(std::string fileName) -> Mesh;
};

