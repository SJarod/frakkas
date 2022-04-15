#include <stb_image.h>
#include <assimp/postprocess.h>

#include "log.hpp"
#include "maths.hpp"

#include "resources/resources_manager.hpp"


Resources::ResourcesManager::~ResourcesManager()
{
    for (auto& texture : textures)
        stbi_image_free(texture.get()->data);
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

        o_mesh.vertices.emplace_back(vertex);
    }

    // process indices
    for (unsigned int i = 0; i < i_aim.mNumFaces; ++i)
    {
        aiFace face = i_aim.mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; ++j)
            o_mesh.indices.emplace_back(face.mIndices[j]);
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

        // load the transposed mesh matrix : (i % 4 * 4) + (i / 4)
        for (int i = 0; i < 16; ++i)
        {
            int j = (i % 4 * 4) + (i / 4);
            mesh.localTransform.element[i] = *(&i_node.mTransformation.a1 + j);
        }

        meshes.emplace_back(std::make_shared<Mesh>(mesh));
    }
    // then do the same for each of its children
    for (unsigned int i = 0; i < i_node.mNumChildren; ++i)
        ProcessAiNode(*i_node.mChildren[i], i_scene, i_embeddedTexture);
}

int Resources::ResourcesManager::LoadCPUModel(Assimp::Importer& io_importer, const std::string& i_path)
{
    io_importer.SetPropertyBool(AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS, false);
    const aiScene* scene = io_importer.ReadFile(i_path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_OptimizeMeshes);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        Log::Error("ASSIMP: " + (std::string)io_importer.GetErrorString());
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
        io_mesh.vertices.emplace_back(rawVertices[io_mesh.indices[i]]);
}

void Resources::ResourcesManager::CreateGPUMesh(Mesh& io_mesh)
{
    ParseMesh(io_mesh);

    glCreateVertexArrays(1, &io_mesh.gpu.VAO);
    glCreateBuffers(1, &io_mesh.gpu.VBO);

    glNamedBufferData(io_mesh.gpu.VBO, io_mesh.vertices.size() * sizeof(Vertex), io_mesh.vertices.data(), GL_STATIC_DRAW);

    glEnableVertexArrayAttrib(io_mesh.gpu.VAO, 0);
    glVertexArrayAttribBinding(io_mesh.gpu.VAO, 0, 0);
    glVertexArrayAttribFormat(io_mesh.gpu.VAO, 0, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, position));

    glEnableVertexArrayAttrib(io_mesh.gpu.VAO, 1);
    glVertexArrayAttribBinding(io_mesh.gpu.VAO, 1, 0);
    glVertexArrayAttribFormat(io_mesh.gpu.VAO, 1, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, normal));

    glEnableVertexArrayAttrib(io_mesh.gpu.VAO, 2);
    glVertexArrayAttribBinding(io_mesh.gpu.VAO, 2, 0);
    glVertexArrayAttribFormat(io_mesh.gpu.VAO, 2, 2, GL_FLOAT, GL_FALSE, offsetof(Vertex, uv));

    glVertexArrayVertexBuffer(io_mesh.gpu.VAO, 0, io_mesh.gpu.VBO, 0, sizeof(Vertex));

    glBindVertexArray(0);
}

std::vector<std::shared_ptr<Mesh>> Resources::ResourcesManager::LoadModel(const std::string& i_filename, const bool i_embeddedTexture)
{
    ResourcesManager rm = ResourcesManager::Instance();

    int meshOffset = rm.meshes.size();

    Assimp::Importer importer;

    if (rm.LoadCPUModel(importer, i_filename.c_str()) == 0)
    {
        const aiScene* scene = importer.GetScene();
        rm.ProcessAiNode(*scene->mRootNode, *scene, i_embeddedTexture);

        for (int i = meshOffset; i < rm.meshes.size(); ++i)
            rm.CreateGPUMesh(*rm.meshes[i].get());

        Log::Info("Successfully loaded model file: \"" + (std::string)i_filename + "\"");
    }
    else
        Log::Warning("Couldn't load model file: \"" + (std::string)i_filename + "\"");

    std::vector<std::shared_ptr<Mesh>> out;

    for (int i = meshOffset; i < rm.meshes.size(); ++i)
        out.emplace_back(rm.meshes[i]);

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

        mesh.vertices.emplace_back(v);
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
        mesh.indices.emplace_back(ind[i]);

    mesh.diffuseTex.data = 0;
    rm.meshes.emplace_back(std::make_shared<Mesh>(mesh));
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
        mesh.indices.emplace_back(i + i_lon);
        mesh.indices.emplace_back(i + i_lon + 1);
    }

    mesh.diffuseTex.data = 0;
    rm.meshes.emplace_back(std::make_shared<Mesh>(mesh));
    rm.CreateGPUMesh(*(rm.meshes.end() - 1)->get());

    return *(rm.meshes.end() - 1);
}

void Resources::ResourcesManager::LoadCPUTexture(const std::string& i_filename, const bool i_flip, const TextureType i_type)
{
    Texture texture;
    stbi_set_flip_vertically_on_load(i_flip);
    texture.data = stbi_load(i_filename.c_str(), &texture.width, &texture.height, &texture.bpp, 0);
    texture.type = i_type;

    if (texture.data)
    {
        textures.emplace_back(std::make_shared<Texture>(texture));
        Log::Info("Successfully loaded texture file: \"" + i_filename + "\"");
    }
    else
        Log::Warning("Couldn't load texture file: \"" + i_filename + "\"");
}

void Resources::ResourcesManager::CreateGPUTexture(Texture& io_texture)
{
    glCreateTextures(GL_TEXTURE_2D, 1, &io_texture.gpu.data);

    glTextureParameteri(io_texture.gpu.data, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTextureParameteri(io_texture.gpu.data, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTextureParameteri(io_texture.gpu.data, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(io_texture.gpu.data, GL_TEXTURE_WRAP_T, GL_REPEAT);

    if (io_texture.data)
    {
        GLint internalFormat = io_texture.bpp == 1 ? GL_R8 : io_texture.bpp == 2 ? GL_RG8 : io_texture.bpp == 3 ? GL_RGB8 : GL_RGBA8;
        GLint format = io_texture.bpp == 1 ? GL_RED : io_texture.bpp == 2 ? GL_RG : io_texture.bpp == 3 ? GL_RGB : GL_RGBA;

        glTextureStorage2D(io_texture.gpu.data, 1, internalFormat, io_texture.width, io_texture.height);
        glTextureSubImage2D(io_texture.gpu.data, 0, 0, 0, io_texture.width, io_texture.height, format, GL_UNSIGNED_BYTE, io_texture.data);

        glGenerateTextureMipmap(io_texture.gpu.data);
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
            CreateGPUTexture(*textures[j].get());
    }
}

std::shared_ptr<Texture> Resources::ResourcesManager::LoadTexture(const std::string& i_filename, const bool i_flip)
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