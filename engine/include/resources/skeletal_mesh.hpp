#pragma once

#include <unordered_map>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>

#include "resources/mesh.hpp"
#include "maths.hpp"

#define MAX_BONES 100

namespace Resources
{
	struct Bone
	{
		int id;
		Matrix4 offset;
	};

	class SkeletalMesh : public Resources::Mesh
	{
	private:
		void ProcessAiMesh(std::shared_ptr<Submesh>& o_submesh,
			const aiMesh& i_aim,
			const aiScene& i_scene) override;

		/**
		 * Set a specific vertex's bone indices and weights.
		 */
		void SetVertexBoneData(Vertex& vertex, const int boneID, const float weight);

		/**
		 * Load bone informations (bone indice and bone weight) for each vertex of the mesh.
		 */
		void ExtractBoneWeightForVertices(std::shared_ptr<Submesh>& o_submesh,
			const aiMesh& i_aim,
			const aiScene& i_scene);

	public:
		// Can the mesh be loaded?
		bool meshSuccess = true;

		std::unordered_map<std::string, Bone> boneInfoMap;
		int boneCounter = 0;

		SkeletalMesh(const std::string& i_name);

		void LoadFromInfo() override;

		void ComputeMemorySize() override;
	};
}