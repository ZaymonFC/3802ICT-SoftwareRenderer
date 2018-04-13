#pragma once
#include "Mesh.h"
#include <map>

class MeshManager
{
	Mesh mesh_;
	std::string meshName_;
	Mesh compass_;
	std::map<char, std::string> meshMap_;
	int frame_wide_;
	int frame_high_;


public:
	MeshManager(int frame_wide, int frame_high);

	auto SwitchMesh(char key) -> void;

	auto CorrectMeshState(char key) -> void;

	auto Rotate(float xAmount, float yAmount, float zAmount) -> void;
	auto Translate(float xAmount, float yAmount, float zAmount) -> void;

	auto GetMeshes() const -> std::vector<Mesh>;

	auto GetText()->std::string;
	auto PrintStatus() const -> void;
	auto Scale(float scaleFactor) -> void;
};

