#include <cassert>

#include "resources/resources_manager.hpp"
#include "debug/log.hpp"

#include "resources/skeletal_mesh.hpp"

SkeletalMesh::SkeletalMesh(const std::string& i_name)
	: Mesh(i_name)
{
}

void SkeletalMesh::LoadFromInfo()
{
	resourceType = EResourceType::SKELETALMESH;

	ResourcesManager::AddCPULoadingTask([this](/*void* userData*/) {
		//ResourceSubmeshTaskData* data = static_cast<ResourceSubmeshTaskData*>(userData);
		// ... data.meshes.emplace_back(); ...

		Assimp::Importer importer;
		importer.SetPropertyBool(AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS, false);
		const aiScene* scene = importer.ReadFile(name, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_OptimizeMeshes);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			Log::Warning("ASSIMP: ", static_cast<std::string>(importer.GetErrorString()));
			meshSuccess = false;
			return;
		}
		else
		{
			std::list<std::shared_ptr<Submesh>> buffer;

			const aiScene* scene = importer.GetScene();
			ProcessAiNode(buffer, importer, scene->mRootNode);

			submeshes = buffer;

			Log::Info("Successfully loaded model file : " + name);

			ComputeMemorySize();
		}
		});
}

void SkeletalMesh::ComputeMemorySize()
{
	ram = 0;
	vram = 0;

	Mesh::ComputeMemorySize();

	for (const auto& info : boneInfoMap)
	{
		ram += sizeof(info);
	}
}

void SkeletalMesh::ProcessAiMesh(std::shared_ptr<Submesh>& o_submesh,
	const aiMesh& i_aim,
	const aiScene& i_scene)
{
	// process vertices
	for (unsigned int i = 0; i < i_aim.mNumVertices; ++i)
	{
		Vertex vertex = { Vector3::zero,
			Vector3::zero,
			{ 0.f, 0.f },
			{ -1, -1, -1, -1 },
			{ 0.f, 0.f, 0.f, 0.f } };

		// process vertex positions, normals and texture coordinates
		vertex.position = { i_aim.mVertices[i].x, i_aim.mVertices[i].y, i_aim.mVertices[i].z };

		if (i_aim.HasNormals())
			vertex.normal = { i_aim.mNormals[i].x, i_aim.mNormals[i].y, i_aim.mNormals[i].z };

		if (i_aim.mTextureCoords[0]) // does the mesh contain texture coordinates?
			vertex.uv = { i_aim.mTextureCoords[0][i].x, i_aim.mTextureCoords[0][i].y };

		o_submesh->vertices.emplace_back(vertex);
	}
	ExtractBoneWeightForVertices(o_submesh, i_aim, i_scene);

	// process indices
	for (unsigned int i = 0; i < i_aim.mNumFaces; ++i)
	{
		aiFace face = i_aim.mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; ++j)
			o_submesh->indices.emplace_back(face.mIndices[j]);
	}
}

void SkeletalMesh::SetVertexBoneData(Vertex& vertex, const int boneID, const float weight)
{
	for (int i = 0; i < 4; ++i)
	{
		if (vertex.boneIndices[i] < 0)
		{
			vertex.boneWeights.element[i] = weight;
			vertex.boneIndices[i] = boneID;
			break;
		}
	}
}

void SkeletalMesh::ExtractBoneWeightForVertices(std::shared_ptr<Submesh>& o_submesh,
	const aiMesh& i_aim,
	const aiScene& i_scene)
{
	for (int boneIndex = 0; boneIndex < i_aim.mNumBones; ++boneIndex)
	{
		int boneID = -1;
		std::string boneName(i_aim.mBones[boneIndex]->mName.C_Str());
		if (boneInfoMap.find(boneName) == boneInfoMap.end())
		{
			Bone newBoneInfo;
			newBoneInfo.id = boneCounter;
			for (int i = 0; i < 16; ++i)
				newBoneInfo.offset.element[i] = *(&i_aim.mBones[boneIndex]->mOffsetMatrix.a1 + i);
			boneInfoMap[boneName] = newBoneInfo;
			boneID = boneCounter;
			++boneCounter;
		}
		else
		{
			boneID = boneInfoMap[boneName].id;
		}
		assert(boneID != -1);
		aiVertexWeight* weights = i_aim.mBones[boneIndex]->mWeights;
		int numWeights = i_aim.mBones[boneIndex]->mNumWeights;

		for (int weightIndex = 0; weightIndex < numWeights; ++weightIndex)
		{
			int vertexId = weights[weightIndex].mVertexId;
			float weight = weights[weightIndex].mWeight;
			assert(vertexId <= o_submesh->vertices.size());
			SetVertexBoneData(o_submesh->vertices[vertexId], boneID, weight);
		}
	}
}
