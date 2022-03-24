//
// Created by m.mehalin on 23/03/2022.
//

#include <stb_image.h>
#include <assimp/postprocess.h>

#include "maths.hpp"

#include "resources/resources_manager.hpp"

Resources::ResourcesManager::~ResourcesManager()
{
    for (auto& texture : textures)
    {
        stbi_image_free(texture.get()->data);
    }
}

void Resources::ResourcesManager::ProcessAiMesh(const aiMesh& i_aim, const aiScene& i_scene, Mesh& o_mesh, const bool i_embeddedTexture)
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

        o_mesh.vertices.push_back(vertex);
    }

    // process indices
    for (unsigned int i = 0; i < i_aim.mNumFaces; ++i)
    {
        aiFace face = i_aim.mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; ++j)
            o_mesh.indices.push_back(face.mIndices[j]);
    }

    if (i_embeddedTexture)
    {
        // process materials/textures
        for (unsigned int i = 0; i < i_scene.mNumMaterials; ++i)
        {
            aiMaterial* material = i_scene.mMaterials[i_aim.mMaterialIndex];

            LoadEmbeddedTexture(*material, i_scene, aiTextureType_DIFFUSE, TextureType::TEXTURE_DIFFUSE);

            // TODO : give mesh an array of textures
        }
    }
    else
        o_mesh.diffuseTex.data = 0;
}

void Resources::ResourcesManager::ProcessAiNode(const aiNode& i_node, const aiScene& i_scene, const bool i_embeddedTexture)
{
    // process all the node's meshes (if any)
    for (unsigned int i = 0; i < i_node.mNumMeshes; ++i)
    {
        aiMesh* aim = i_scene.mMeshes[i_node.mMeshes[i]];

        Mesh mesh;
        ProcessAiMesh(*aim, i_scene, mesh, i_embeddedTexture);

        meshes.push_back(std::make_shared<Mesh>(mesh));
    }
    // then do the same for each of its children
    for (unsigned int i = 0; i < i_node.mNumChildren; ++i)
    {
        ProcessAiNode(*i_node.mChildren[i], i_scene, i_embeddedTexture);
    }
}

int Resources::ResourcesManager::LoadCPUModel(Assimp::Importer& io_importer, const char* i_path)
{
    io_importer.SetPropertyBool(AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS, false);
    const aiScene* scene = io_importer.ReadFile(i_path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_OptimizeMeshes);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "ERROR::ASSIMP::" << io_importer.GetErrorString() << std::endl;
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
    ParseMesh(io_mesh);

    glGenBuffers(1, &io_mesh.gpu.VBO);
    glBindBuffer(GL_ARRAY_BUFFER, io_mesh.gpu.VBO);
    glBufferData(GL_ARRAY_BUFFER, io_mesh.vertices.size() * sizeof(Vertex), io_mesh.vertices.data(), GL_STATIC_DRAW);

    glGenVertexArrays(1, &io_mesh.gpu.VAO);
    glBindVertexArray(io_mesh.gpu.VAO);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));

    glBindVertexArray(0);
}

std::vector<std::shared_ptr<Mesh>> Resources::ResourcesManager::LoadModel(const char* i_filename, const bool i_embeddedTexture)
{
    std::string f = i_filename;
    f = "../assets/" + f;

    ResourcesManager rm = ResourcesManager::Instance();

    int meshOffset = rm.meshes.size();

    Assimp::Importer importer;
    if (rm.LoadCPUModel(importer, f.c_str()) == 0)
    {
        const aiScene* scene = importer.GetScene();
        rm.ProcessAiNode(*scene->mRootNode, *scene, i_embeddedTexture);

        for (int i = meshOffset; i < rm.meshes.size(); ++i)
        {
            rm.CreateGPUMesh(*rm.meshes[i].get());
        }

        std::cout << "Successfully loaded model file : " << i_filename << std::endl;
    }
    else
    {
        std::cout << "Could not load model file : " << i_filename << std::endl;
    }

    std::vector<std::shared_ptr<Mesh>> out;
    for (int i = meshOffset; i < rm.meshes.size(); ++i)
    {
        out.push_back(rm.meshes[i]);
    }
    return out;
}

std::shared_ptr<Mesh> Resources::ResourcesManager::LoadCube()
{
    ResourcesManager rm = ResourcesManager::Instance();

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

    mesh.diffuseTex.data = 0;
    rm.meshes.push_back(std::make_shared<Mesh>(mesh));
    rm.CreateGPUMesh(*(rm.meshes.end() - 1)->get());

    return *(rm.meshes.end() - 1);
}

std::shared_ptr<Mesh> Resources::ResourcesManager::LoadSphere(const float i_radius, const int i_lon, const int i_lat)
{
    ResourcesManager rm = ResourcesManager::Instance();

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

    mesh.diffuseTex.data = 0;
    rm.meshes.push_back(std::make_shared<Mesh>(mesh));
    rm.CreateGPUMesh(*(rm.meshes.end() - 1)->get());

    return *(rm.meshes.end() - 1);
}

void Resources::ResourcesManager::LoadCPUTexture(const char* i_filename, const bool i_flip, const TextureType i_type)
{
    std::string f = i_filename;

    Texture texture;
    stbi_set_flip_vertically_on_load(i_flip);
    texture.data = stbi_load(f.c_str(), &texture.width, &texture.height, &texture.bpp, 0);
    texture.type = i_type;

    if (texture.data)
    {
        textures.push_back(std::make_shared<Texture>(texture));
        std::cout << "Successfully loaded texture file : " << f << std::endl;
    }
    else
        std::cout << "Could not load texture file : " << f << std::endl;
}

void Resources::ResourcesManager::CreateGPUTexture(Texture& io_texture)
{
    glGenTextures(1, &io_texture.gpu.data);

    glBindTexture(GL_TEXTURE_2D, io_texture.gpu.data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    if (io_texture.data)
    {
        GLint bpp = io_texture.bpp == 1 ? GL_RED : io_texture.bpp == 2 ? GL_RG : io_texture.bpp == 3 ? GL_RGB : GL_RGBA;
        glTexImage2D(GL_TEXTURE_2D, 0, bpp, io_texture.width, io_texture.height, 0, bpp, GL_UNSIGNED_BYTE, io_texture.data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
}

void Resources::ResourcesManager::LoadEmbeddedTexture(const aiMaterial& i_mat, const aiScene& i_scene, const aiTextureType i_aiType, const TextureType i_type)
{
    int textureOffset = textures.size();
    //scene->
    for (unsigned int i = 0; i < i_mat.GetTextureCount(i_aiType); ++i)
    {
        aiString str;
        i_mat.GetTexture(i_aiType, i, &str);
        LoadCPUTexture(str.C_Str(), false, i_type);

        for (int j = textureOffset; j < textures.size(); ++j)
        {
            CreateGPUTexture(*textures[j].get());
        }
    }
}

std::shared_ptr<Texture> Resources::ResourcesManager::LoadTexture(const char* i_filename, const bool i_flip)
{
    ResourcesManager rm = ResourcesManager::Instance();

    rm.LoadCPUTexture(i_filename, i_flip);
    rm.CreateGPUTexture(*rm.textures.back().get());

    return rm.textures.back();
}

unsigned int Resources::ResourcesManager::GetLoadedMeshCount()
{
    ResourcesManager rm = ResourcesManager::Instance();
    return rm.meshes.size();
}

unsigned int Resources::ResourcesManager::GetLoadedTextureCount()
{
    ResourcesManager rm = ResourcesManager::Instance();
    return rm.textures.size();
}