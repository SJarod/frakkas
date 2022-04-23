#include <stb_image.h>
#include <assimp/postprocess.h>

#include "debug/log.hpp"
#include "maths.hpp"

#include "resources/resources_manager.hpp"

Resources::ResourcesManager::~ResourcesManager()
{
	for (auto& resource : resources)
	{
		if (std::shared_ptr<Texture> texture = std::dynamic_pointer_cast<Texture>(resource.second))
			if (texture->data)
				stbi_image_free(texture->data);
	}
}

const std::string Resources::ResourcesManager::pathToShaders = "engine/shaders/";
const std::string Resources::ResourcesManager::pathToAssets = "game/assets/";

void Resources::ResourcesManager::ProcessAiMesh(std::shared_ptr<Mesh>& o_mesh,
	const aiMesh& i_aim,
	const aiScene& i_scene,
	const std::string& i_textureFilename,
	const bool i_flipTexture)
{
	// process vertices
	for (unsigned int i = 0; i < i_aim.mNumVertices; ++i)
	{
		Vertex vertex = { Vector3::zero, Vector3::zero, { 0.f, 0.f } };

		// process vertex positions, normals and texture coordinates
		vertex.position = { i_aim.mVertices[i].x, i_aim.mVertices[i].y, i_aim.mVertices[i].z };

		if (i_aim.HasNormals())
			vertex.normal = { i_aim.mNormals[i].x, i_aim.mNormals[i].y, i_aim.mNormals[i].z };

		if (i_aim.mTextureCoords[0]) // does the mesh contain texture coordinates?
			vertex.uv = { i_aim.mTextureCoords[0][i].x, i_aim.mTextureCoords[0][i].y };

		o_mesh->vertices.push_back(vertex);
	}

	// process indices
	for (unsigned int i = 0; i < i_aim.mNumFaces; ++i)
	{
		aiFace face = i_aim.mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; ++j)
			o_mesh->indices.push_back(face.mIndices[j]);
	}

	//if no texture file specified, try to load embedded textures
	if (i_textureFilename == "")
	{
		// process materials/textures
		for (unsigned int i = 0; i < i_scene.mNumMaterials; ++i)
		{
			aiMaterial* material = i_scene.mMaterials[i_aim.mMaterialIndex];

			// TODO : give mesh an array of textures
			LoadEmbeddedTexture(o_mesh->diffuseTex, *material, i_scene, aiTextureType_DIFFUSE, ETextureType::TEXTURE_DIFFUSE);
		}
	}
	else
	{
		o_mesh->diffuseTex = ResourcesManager::LoadResource<Texture>(i_textureFilename, i_flipTexture);
	}
}

void Resources::ResourcesManager::ProcessAiNode(std::list<std::shared_ptr<Mesh>>& o_meshes,
	const std::shared_ptr<Assimp::Importer> i_importer,
	const aiNode* i_node,
	const std::string& i_textureFilename,
	const bool i_flipTexture)
{
	// process all the node's meshes (if any)
	for (unsigned int i = 0; i < i_node->mNumMeshes; ++i)
	{
		const aiScene* scene = i_importer->GetScene();
		const aiMesh* aim = scene->mMeshes[i_node->mMeshes[i]];

		resourceMX.lock();
		if (resources.find(aim->mName.C_Str()) != resources.end())
		{
			o_meshes.push_back(std::static_pointer_cast<Mesh>(resources[aim->mName.C_Str()]));
			resourceMX.unlock();
			continue;
		}
		std::shared_ptr<Mesh> meshPtr = std::make_shared<Mesh>();
		resources[aim->mName.C_Str()] = meshPtr;
		resourceMX.unlock();

		ProcessAiMesh(meshPtr, *aim, *scene, i_textureFilename, i_flipTexture);

		// load the transposed mesh matrix : (i % 4 * 4) + (i / 4)
		for (int i = 0; i < 16; ++i)
		{
			int j = (i % 4 * 4) + (i / 4);
			meshPtr->localTransform.element[i] = *(&i_node->mTransformation.a1 + j);
		}

		ParseMesh(*meshPtr);
		o_meshes.push_back(meshPtr);

		CreateGPUMesh(*o_meshes.back());
	}

	// then do the same for each of its children
	for (unsigned int i = 0; i < i_node->mNumChildren; ++i)
	{
		ProcessAiNode(o_meshes, i_importer, i_node->mChildren[i], i_textureFilename, i_flipTexture);
	}
}

int Resources::ResourcesManager::LoadCPUModel(Assimp::Importer& io_importer, const std::string& i_path)
{
	io_importer.SetPropertyBool(AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS, false);
	const aiScene* scene = io_importer.ReadFile(i_path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_OptimizeMeshes);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::lock_guard<std::mutex> guard(printMX);
		Log::Warning("ASSIMP: " + (std::string)io_importer.GetErrorString());
		return -1;
	}

	return 0;
}

void Resources::ResourcesManager::ParseMesh(Mesh& io_mesh)
{
	std::vector<Vertex> rawVertices = io_mesh.vertices;
	io_mesh.vertices.clear();

	// process indices
	int meshNumFaces = io_mesh.indices.size();
	for (unsigned int i = 0; i < meshNumFaces; ++i)
	{
		io_mesh.vertices.push_back(rawVertices[io_mesh.indices[i]]);
	}
}

void Resources::ResourcesManager::CreateGPUMesh(Mesh& io_mesh)
{
	ResourcesManager& rm = ResourcesManager::Instance();

	std::lock_guard<std::mutex> guard(rm.loadMX);
	rm.gpuLoadQueue.push_back([&]() {
		glGenBuffers(1, &io_mesh.gpu.VBO);
		glBindBuffer(GL_ARRAY_BUFFER, io_mesh.gpu.VBO);
		glBufferData(GL_ARRAY_BUFFER, io_mesh.vertices.size() * sizeof(Vertex), io_mesh.vertices.data(), GL_STATIC_DRAW);

		glGenVertexArrays(1, &io_mesh.gpu.VAO);
		glBindVertexArray(io_mesh.gpu.VAO);

#if 1
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
#else
		glVertexAttrib3f(0, 0.f, 0.f, 0.f);
		glVertexAttrib3f(1, 0.f, 0.f, 0.f);
		glVertexAttrib2f(2, 0.f, 0.f);
#endif

		glBindVertexArray(0);
		});
}

void Resources::ResourcesManager::LoadModel(std::list<std::shared_ptr<Mesh>>& o_meshes,
	const std::string& i_filename,
	const std::string& i_textureFilename,
	const bool i_flipTexture)
{
	ResourcesManager& rm = ResourcesManager::Instance();

	rm.tp.AddTask([&o_meshes, i_filename, i_textureFilename, i_flipTexture](/*void* userData*/) {
		ResourcesManager& rm = ResourcesManager::Instance();

		//ResourceMeshTaskData* data = static_cast<ResourceMeshTaskData*>(userData);
		// ... data.meshes.push_back(); ...

		std::shared_ptr<Assimp::Importer> importer = std::make_shared<Assimp::Importer>();
		if (rm.LoadCPUModel(*importer, i_filename.c_str()) == 0)
		{
			const aiScene* scene = importer->GetScene();
			//o_meshes.reserve(scene->mNumMeshes * sizeof(std::shared_ptr<Mesh>));
			//o_meshes.reserve(10000);
			rm.ProcessAiNode(o_meshes, importer, scene->mRootNode, i_textureFilename, i_flipTexture);

			std::lock_guard<std::mutex> guard(rm.printMX);
			Log::Info("Successfully loaded model file : " + i_filename);
		}
		else
		{
			std::lock_guard<std::mutex> guard(rm.printMX);
			Log::Warning("Could not load model file : " + i_filename);
		}
		});
}

void Resources::ResourcesManager::LoadCube(std::list<std::shared_ptr<Mesh>>& o_meshes)
{
	ResourcesManager& rm = ResourcesManager::Instance();

	if (rm.resources.find("ProceduralCube") != rm.resources.end())
	{
		o_meshes.push_back(std::static_pointer_cast<Mesh>(rm.resources["ProceduralCube"]));
		return;
	}

	Mesh mesh;

	float ver[] = {
			-1.f,  1.f,  1.f,
			1.f,  1.f,  1.f,
			1.f, -1.f,  1.f,
			-1.f, -1.f,  1.f,
			-1.f, -1.f, -1.f,
			-1.f,  1.f, -1.f,
			1.f,  1.f, -1.f,
			1.f, -1.f, -1.f,
	};

	for (int i = 0; i < 8; ++i)
	{
		Vertex v;
		v.position = { ver[i * 3 + 0], ver[i * 3 + 1], ver[i * 3 + 2] };

		mesh.vertices.push_back(v);
	}

	unsigned int ind[] = {
			0, 1, 3,
			1, 2, 3,
			5, 6, 4,
			6, 4, 7,
			0, 1, 5,
			1, 5, 6,
			2, 3, 4,
			2, 4, 7,
			0, 3, 4,
			0, 4, 5,
			1, 2, 7,
			1, 6, 7,
	};

	for (int i = 0; i < 36; ++i)
	{
		mesh.indices.push_back(ind[i]);
	}

	rm.resources["ProceduralCube"] = std::make_shared<Mesh>(mesh);

	std::shared_ptr<Mesh> m = std::static_pointer_cast<Mesh>(rm.resources["ProceduralCube"]);
	rm.CreateGPUMesh(*m);

	o_meshes.push_back(m);
}

void Resources::ResourcesManager::LoadSphere(std::list<std::shared_ptr<Mesh>>& o_meshes,
	const float i_radius,
	const int i_lon,
	const int i_lat)
{
	ResourcesManager& rm = ResourcesManager::Instance();

	if (rm.resources.find("ProceduralSphere") != rm.resources.end())
	{
		o_meshes.push_back(std::static_pointer_cast<Mesh>(rm.resources["ProceduralSphere"]));
		return;
	}

	Mesh mesh;

	float r = i_radius;

	float angleLon = 2.f * Maths::Constants::pi / i_lon;
	float angleLat = Maths::Constants::pi / i_lat;

	for (int i = 0; i < i_lat; ++i)
	{
		for (int j = 0; j < i_lon; ++j)
		{
			Vertex v;
			v.position = { r * Maths::Cos(j * angleLon) * Maths::Sin(i * angleLat),
						   r * Maths::Cos(i * angleLat),
						   r * Maths::Sin(j * angleLon) * Maths::Sin(i * angleLat) };
			v.normal = v.position;
			v.uv = { j / (float)i_lon, i / (float)i_lat };

			mesh.vertices.push_back(v);
		}
	}

	for (int i = 0; i < mesh.vertices.size() - i_lon; ++i)
	{
		// disposition of vertex
		// i            i + 1
		// i + lon		i + lon + 1

		mesh.indices.push_back(i);
		mesh.indices.push_back(i + 1);
		mesh.indices.push_back(i + i_lon);

		if (i == mesh.vertices.size() - i_lon - 1)
			continue;

		mesh.indices.push_back(i + 1);
		mesh.indices.push_back(i + i_lon);
		mesh.indices.push_back(i + i_lon + 1);
	}

	rm.resources["ProceduralSphere"] = std::make_shared<Mesh>(mesh);

	std::shared_ptr<Mesh> m = std::static_pointer_cast<Mesh>(rm.resources["ProceduralSphere"]);
	rm.CreateGPUMesh(*m);

	o_meshes.push_back(m);
}

void Resources::ResourcesManager::CreateGPUTexture(Texture& io_texture)
{
	ResourcesManager& rm = ResourcesManager::Instance();

	std::lock_guard<std::mutex> guard(rm.loadMX);
	rm.gpuLoadQueue.push_back([&]() {
		io_texture.gpu = std::make_unique<GPUTexture>();
		glGenTextures(1, &io_texture.gpu->data);

		glBindTexture(GL_TEXTURE_2D, io_texture.gpu->data);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		if (io_texture.data)
		{
			GLint bpp = io_texture.bpp == 1 ? GL_RED : io_texture.bpp == 2 ? GL_RG : io_texture.bpp == 3 ? GL_RGB : GL_RGBA;
			glTexImage2D(GL_TEXTURE_2D, 0, bpp, io_texture.width, io_texture.height, 0, bpp, GL_UNSIGNED_BYTE, io_texture.data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		});
}

const DefaultTexture& Resources::ResourcesManager::GetDefaultTexture()
{
	return ResourcesManager::Instance().defaultTexture;
}

void Resources::ResourcesManager::AddCPULoadingTask(Task io_task)
{
	ResourcesManager::Instance().tp.AddTask(io_task);
}

void Resources::ResourcesManager::PollGPULoad()
{
	ResourcesManager& rm = ResourcesManager::Instance();

	std::lock_guard<std::mutex> guard(rm.loadMX);

	for (Task& task : rm.gpuLoadQueue)
	{
		task();
	}

	rm.gpuLoadQueue.clear();
}

void Resources::ResourcesManager::LoadEmbeddedTexture(std::shared_ptr<Texture>& o_texture,
	const aiMaterial& i_mat,
	const aiScene& i_scene,
	const aiTextureType i_aiType,
	const ETextureType i_type)
{
	for (unsigned int i = 0; i < i_mat.GetTextureCount(i_aiType); ++i)
	{
		aiString str;
		i_mat.GetTexture(i_aiType, i, &str);

		o_texture = ResourcesManager::LoadResource<Texture>(std::string(str.C_Str()), false);
	}
}
