#include "resources/resources_manager.hpp"
#include "debug/log.hpp"

#include "resources/mesh.hpp"

Resources::Mesh::Mesh(const std::string& i_name)
{
	name = i_name;
}

Resources::Mesh::Mesh(const std::string& i_name, const std::string& i_textureFilename, const bool i_flipTexture)
	: textureName(i_textureFilename), flipTexture(i_flipTexture)
{
	name = i_name;
}

void Resources::Mesh::LoadFromInfo()
{
	if (name == cubeMesh)
		LoadCube();
    if (name == cubeColliderMesh)
        LoadLineCube();
	else if (name == sphereMesh)
		LoadSphere();
    else if (name == sphereColliderMesh)
        LoadLineSphere();
	else
		ResourcesManager::AddCPULoadingTask([this](/*void* userData*/) {
			//ResourceSubmeshTaskData* data = static_cast<ResourceSubmeshTaskData*>(userData);
			// ... data.meshes.emplace_back(); ...

			Assimp::Importer importer;
			importer.SetPropertyBool(AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS, false);
			const aiScene* scene = importer.ReadFile(name, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_OptimizeMeshes);

			if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
			{
				Log::Warning("Could not load model file : " + name + "\nASSIMP: " + (std::string)importer.GetErrorString());
				return;
			}
			else
			{
				std::list<std::shared_ptr<Submesh>> buffer;

				const aiScene* scene = importer.GetScene();
				ProcessAiNode(buffer, importer, scene->mRootNode);

				submeshes = buffer;

				Log::Info("Successfully loaded model file : " + name);
			}
			});
}

void Resources::Mesh::ProcessAiMesh(std::shared_ptr<Submesh>& o_mesh,
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

		o_mesh->vertices.emplace_back(vertex);
	}

	// process indices
	for (unsigned int i = 0; i < i_aim.mNumFaces; ++i)
	{
		aiFace face = i_aim.mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; ++j)
			o_mesh->indices.emplace_back(face.mIndices[j]);
	}

	//if no texture file specified, try to load embedded textures
	if (textureName == "")
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
		o_mesh->diffuseTex = ResourcesManager::LoadResource<Texture>(textureName, flipTexture);
	}
}

void Resources::Mesh::ProcessAiNode(std::list<std::shared_ptr<Submesh>>& o_meshes,
	const Assimp::Importer& i_importer,
	const aiNode* i_node)
{
	// process all the node's meshes (if any)
	for (unsigned int i = 0; i < i_node->mNumMeshes; ++i)
	{
		const aiScene* scene = i_importer.GetScene();
		const aiMesh* aim = scene->mMeshes[i_node->mMeshes[i]];

		std::shared_ptr<Submesh> meshPtr = std::make_shared<Submesh>();

		ProcessAiMesh(meshPtr, *aim, *scene);

		// load the transposed mesh matrix : (i % 4 * 4) + (i / 4)
		for (int i = 0; i < 16; ++i)
		{
			int j = (i % 4 * 4) + (i / 4);
			meshPtr->localTransform.element[i] = *(&i_node->mTransformation.a1 + j);
		}

		ParseSubmesh(*meshPtr);
		o_meshes.emplace_back(meshPtr);

		ResourcesManager::CreateGPUSubmesh(*o_meshes.back());
	}

	// then do the same for each of its children
	for (unsigned int i = 0; i < i_node->mNumChildren; ++i)
	{
		ProcessAiNode(o_meshes, i_importer, i_node->mChildren[i]);
	}
}

void Resources::Mesh::ParseSubmesh(Submesh& io_mesh)
{
	std::vector<Vertex> rawVertices = io_mesh.vertices;
	io_mesh.vertices.clear();

	// process indices
	int meshNumFaces = io_mesh.indices.size();
	for (unsigned int i = 0; i < meshNumFaces; ++i)
	{
		io_mesh.vertices.emplace_back(rawVertices[io_mesh.indices[i]]);
	}
}

void Resources::Mesh::LoadEmbeddedTexture(std::shared_ptr<Texture>& o_texture,
	const aiMaterial& i_mat,
	const aiScene& i_scene,
	const aiTextureType i_aiType,
	const ETextureType i_type)
{
	for (unsigned int i = 0; i < i_mat.GetTextureCount(i_aiType); ++i)
	{
		aiString str;
		i_mat.GetTexture(i_aiType, i, &str);

		o_texture = ResourcesManager::LoadResource<Texture>(std::string(str.C_Str()), flipTexture);
	}
}

void Resources::Mesh::LoadCube()
{
	Submesh mesh;

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

		mesh.vertices.emplace_back(v);
	}

	unsigned int ind[] = {
		2,1,0,
		0,3,2,
		5,6,7,
		7,4,5,
		3,4,7,
		7,2,3,
		6,5,0,
		0,1,6,
		1,2,7,
		7,6,1,
		0,5,4,
		4,3,0,
	};

	for (int i = 0; i < 36; ++i)
	{
		mesh.indices.emplace_back(ind[i]);
	}

	ParseSubmesh(mesh);
	submeshes.emplace_back(std::make_shared<Submesh>(mesh));
	ResourcesManager::CreateGPUSubmesh(*submeshes.back());
}

void Mesh::LoadLineCube()
{
    Submesh mesh;

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

        mesh.vertices.emplace_back(v);
    }

    unsigned int ind[] = {
            0, 1,
            2, 3,
            0, 5,
            6, 7,
            4, 5,
            6, 1,
            2, 7,
            4, 3
    };

    for (int i = 0; i < 16; ++i)
    {
        mesh.indices.emplace_back(ind[i]);
    }

    ParseSubmesh(mesh);
    submeshes.emplace_back(std::make_shared<Submesh>(mesh));
    ResourcesManager::CreateGPUSubmesh(*submeshes.back());
}

void Resources::Mesh::LoadSphere(const float i_radius,
	const int i_lon,
	const int i_lat)
{
	Submesh mesh;

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

			mesh.vertices.emplace_back(v);
		}
	}

	for (int i = 0; i < mesh.vertices.size() - i_lon; ++i)
	{
		// disposition of vertex
		// i            i + 1
		// i + lon		i + lon + 1

		mesh.indices.emplace_back(i);
		mesh.indices.emplace_back(i + 1);
		mesh.indices.emplace_back(i + i_lon);

		if (i == mesh.vertices.size() - i_lon - 1)
			continue;

		mesh.indices.emplace_back(i + 1);
		mesh.indices.emplace_back(i + i_lon + 1);
		mesh.indices.emplace_back(i + i_lon);
	}

	ParseSubmesh(mesh);
	submeshes.emplace_back(std::make_shared<Submesh>(mesh));
	ResourcesManager::CreateGPUSubmesh(*submeshes.back());
}

void Mesh::LoadLineSphere(float i_radius)
{
    Submesh mesh;

    float r = i_radius;
    float res = 30;

    float angle = Maths::Constants::doublePi / res;

    Vertex v;
    // X axis
    for (int i = 0; i < res; i++)
    {
        v.position = { 0.f, r * Maths::Sin(i * angle), r * Maths::Cos(i * angle) };
        mesh.vertices.emplace_back(v);
        if ( i < res-1)
        {
            mesh.indices.emplace_back(i);
            mesh.indices.emplace_back(i+1);
        }
        else
        {
            mesh.indices.emplace_back(i);
            mesh.indices.emplace_back(0);
        }
    }


    // Y axis
    for (int i = 0; i < res; i++)
    {
        v.position = { r * Maths::Sin(i * angle), 0.f, r * Maths::Cos(i * angle) };
        mesh.vertices.emplace_back(v);
        if ( i < res-1)
        {
            mesh.indices.emplace_back(i+res);
            mesh.indices.emplace_back(i+res+1);
        }
        else
        {
            mesh.indices.emplace_back(i+res);
            mesh.indices.emplace_back(0+res);
        }
    }
    // Z axis
    for (int i = 0; i < res; i++)
    {
        v.position = { r * Maths::Cos(i * angle), r * Maths::Sin(i * angle), 0.f };
        mesh.vertices.emplace_back(v);
        if ( i < res-1)
        {
            mesh.indices.emplace_back(i+res+res);
            mesh.indices.emplace_back(i+res+res+1);
        }
        else
        {
            mesh.indices.emplace_back(i+res+res);
            mesh.indices.emplace_back(0+res+res);
        }
    }

    ParseSubmesh(mesh);
    submeshes.emplace_back(std::make_shared<Submesh>(mesh));
    ResourcesManager::CreateGPUSubmesh(*submeshes.back());
}
