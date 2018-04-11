#include "MeshManager.h"
#include "MeshLoader.h"
#include "GraphicsMath.h"

MeshManager::MeshManager(const int frame_wide, const int frame_high) : frame_wide_{ frame_wide }, frame_high_{ frame_high }
{
	compass_ = MeshLoader::LoadMesh("Objects/compass.json");
	compass_.Translate(frame_wide_ - 100, frame_high_ - 100, 500);
	compass_.MutableRotate(GraphicsMath::Degrees(90), 0, GraphicsMath::Degrees(90));

	compass_.Scale(3);


	meshMap_ = {
		{'1', "Objects/polyhedron.json"},
		{'2', "Objects/magnolia.json"},
		{'3', "Objects/cessna.json"},
		{'4', "Objects/object_text.json"},
		{'5', "Objects/shuttle.json"},
		{'6', "Objects/alfa147.json"},
	};

	SwitchMesh('1');
}

auto MeshManager::SwitchMesh(const char key) -> void
{
	mesh_ = MeshLoader::LoadMesh(meshMap_[key]);
	meshName_ = meshMap_[key];

	CorrectMeshState(key);
}

/**
 * Sets the initial mesh position and rotation to a normalized value, resets compass
 */
auto MeshManager::CorrectMeshState(const char key) -> void
{
	compass_.ResetRotation();

	mesh_.Translate(frame_wide_ / 2, frame_high_ / 2, 500);

	// Normalization for size and rotation between models
	switch (key)
	{
		case '1': // Polyhedron
			mesh_.MutableScale(2);
			break;
		case '2': // Magnolia
			mesh_.MutableScale(5);
			mesh_.MutableRotate(GraphicsMath::Degrees(180), 0.0f, 0.0f);
			break;
		case '3': // Cessna
			mesh_.MutableScale(20);
			mesh_.MutableRotate(GraphicsMath::Degrees(180), 0.0f, 0.0f);
			break;
		case '4': // ObjectText
			mesh_.MutableScale(1.5);
			mesh_.MutableRotate(GraphicsMath::Degrees(90), 0.0f, GraphicsMath::Degrees(-90));
			break;
		case '5': // Shuttle
			mesh_.MutableScale(70);
			mesh_.MutableRotate(GraphicsMath::Degrees(90), 0.0f, 0.0f);
			break;
		case '6': // Alpha147
			mesh_.MutableScale(6);
			mesh_.Translate(30, 110, 0);
			mesh_.MutableRotate(GraphicsMath::Degrees(90), 0.0f, GraphicsMath::Degrees(-45));
			break;
		default:
			break;

	}
	
}

auto MeshManager::Rotate(const float xAmount, const float yAmount, const float zAmount) -> void
{
	mesh_.Rotate(xAmount, yAmount, zAmount);
	compass_.Rotate(xAmount, yAmount, zAmount);
}

auto MeshManager::Translate(float xAmount, float yAmount, float zAmount) -> void
{
	mesh_.Translate(xAmount, yAmount, zAmount);
}

auto MeshManager::GetMeshes() const -> std::vector<Mesh>
{
	auto meshes = std::vector<Mesh>();
	meshes.push_back(mesh_);
	meshes.push_back(compass_);

	return meshes;
}

auto MeshManager::GetText() -> std::string
{
	return std::string();
}

auto MeshManager::PrintStatus() const -> void
{
	mesh_.PrintStatus();
}

auto MeshManager::Scale(float scaleFactor) -> void
{
	mesh_.Scale(scaleFactor);
}
