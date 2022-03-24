//
// Created by m.mehalin on 23/03/2022.
//

#pragma once

#include <vector>
#include <memory>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>

#include "singleton.hpp"
#include "resources/mesh.hpp"
#include "resources/texture.hpp"

using namespace Resources;

namespace Resources
{
    class ResourcesManager : public Singleton<ResourcesManager>
    {
        friend class Singleton<ResourcesManager>;

    public:
        ResourcesManager() = default;
        ~ResourcesManager();

        /**
         * Load a model from a 3D model file.
         * Tell Assimp to load embedded textures if the 3D model file has.
         *
         * @param i_filename
         * @param i_embeddedTexture
         *
         * @return Array of std::shared_ptr<Mesh>
         */
        static std::vector<std::shared_ptr<Mesh>> LoadModel(const char* i_filename, const bool i_embeddedTexture);

        /**
         * Procedurally load a cube.
         *
         * @return std::shared_ptr<Mesh>
         */
        static std::shared_ptr<Mesh> LoadCube();

        /**
         * Procedurally load a sphere.
         *
         * @param i_radius
         * @param i_lon
         * @param i_lat
         *
         * @return std::shared_ptr<Mesh>
         */
        static std::shared_ptr<Mesh> LoadSphere(const float i_radius = 1.f, const int i_lon = 50, const int i_lat = 25);

        /**
         * Load a texture from texture file.
         *
         * @param i_filename
         * @param i_flip
         *
         * @return std::shared_ptr<Texture>
         */
        static std::shared_ptr<Texture> LoadTexture(const char* i_filename, const bool i_flip);

        /**
         * Get the number of loaded mesh.
         *
         */
        static unsigned int GetLoadedMeshCount();

        /**
         * Get the number of loaded texture.
         *
         */
        static unsigned int GetLoadedTextureCount();

    private:
        /**
         * Extrat Assimp's mesh data (vertices and textures).
         * Load mesh's textures if it has embedded textures (specify with a bool).
         *
         * @param i_aiMesh (from Assimp)
         * @param i_aiScene
         * @param o_mesh
         * @param i_embeddedTexture
         */
        void ProcessAiMesh(const aiMesh& i_aim, const aiScene& i_scene, Mesh& o_mesh, const bool i_embeddedTexture);

        /**
         * Go through every Assimp node to extract data (meshes).
         * Tell ProcessAiMesh() to load embedded textures or not.
         *
         * @param i_node
         * @param i_scene
         * @param i_embeddedTexture
         */
        void ProcessAiNode(const aiNode& i_node, const aiScene& i_scene, const bool i_embeddedTexture);

        /**
         * Load a model from a file (group of meshes) into Assimp's importer.
         *
         * @param io_importer
         * @param i_path
         *
         * @return Success
         */
        int LoadCPUModel(Assimp::Importer& io_importer, const char* i_path);

        /**
         * Reorder mesh's vertices with its stored indices.
         *
         * @param io_mesh
         */
        void ParseMesh(Mesh& io_mesh);

        /**
         * Create a mesh GPU side with OpenGL.
         *
         * @param io_mesh
         */
        void CreateGPUMesh(Mesh& io_mesh);

        /**
         * Load texture data with stb_image.
         *
         * @param i_filename
         * @param i_flip
         * @param i_type
         */
        void LoadCPUTexture(const char* i_filename, const bool i_flip, const TextureType i_type = TextureType::TEXTURE_DIFFUSE);

        /**
         * Create a texture GPU side with OpenGL.
         *
         * @param io_texture
         */
        void CreateGPUTexture(Texture& io_texture);

        // TODO : textures are not taken
        // TODO : return vector of textures
        /**
         * Load texture from Assimp's meshes.
         *
         * @param i_aiMaterial
         * @param i_aiScene
         * @param i_aiType
         * @param i_type
         */
        void LoadEmbeddedTexture(const aiMaterial& i_mat, const aiScene& i_scene, const aiTextureType i_aiType, const TextureType i_type);

        // Array of every loaded mesh.
        std::vector<std::shared_ptr<Mesh>> meshes;

        // Array of every loaded textures.
        std::vector<std::shared_ptr<Texture>> textures;
    };
}