#pragma once

#include <unordered_map>
#include <memory>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>

#include "utils/singleton.hpp"
#include "multithread/threadpool.hpp"

#include "resources/mesh.hpp"
#include "resources/texture.hpp"

#include "resources/resource.hpp"

using namespace Resources;

namespace Resources
{
	class ResourcesManager : public Singleton<ResourcesManager>
	{
		friend class Singleton<ResourcesManager>;

	public:
		static const std::string pathToShaders;
		static const std::string pathToAssets;

		const DefaultTexture defaultTexture;

		~ResourcesManager();

		/**
		* Add a resource to the resources map and return it.
		* Return an already existing resource if any.
		* 
		* @param i_name : the resource name or the resource path
		*/
		template<class TResource, typename... TExtraParams> requires std::constructible_from<TResource, const std::string, TExtraParams...>
		static std::shared_ptr<TResource> LoadResource(const std::string& i_name, const TExtraParams&... i_params) noexcept;

		/**
		 * Load a model from a 3D model file into an array (vector) of meshes.
		 * If there is no texture specified, tell Assimp to load embedded textures if the 3D model file has.
		 *
		 * @param o_meshes
		 * @param i_filename the model's filename
		 * @param i_textureFilename the texture's filename, by default set to ""
		 * @param i_flipTexture
		 *
		 */
		static void LoadModel(std::list<std::shared_ptr<Mesh>>& o_meshes,
			const std::string& i_filename,
			const std::string& i_textureFilename = "",
			const bool i_flipTexture = false);

		/**
		 * Procedurally load a cube into the given array of meshes.
		 *
		 * @param o_meshes
		 */
		static void LoadCube(std::list<std::shared_ptr<Mesh>>& o_meshes);

		/**
		 * Procedurally load a sphere into the given array of meshes.
		 *
		 * @param o_meshes
		 * @param i_radius
		 * @param i_longitude
		 * @param i_latitude
		 *
		 */
		static void LoadSphere(std::list<std::shared_ptr<Mesh>>& o_meshes,
			const float i_radius = 1.f,
			const int i_longitude = 50,
			const int i_latitude = 25);

		/**
		* Add a task to the resources manager's thread pool.
		*
		*/
		static void AddCPULoadingTask(Task io_task);

		/**
		* Load GPU data if they are added to the GPU load queue.
		*/
		static void PollGPULoad();

		/**
		 * Add a task to the GPU load queue to create a GPU mesh with OpenGL
		 * in the PollGPULoad() function.
		 *
		 * @param io_mesh
		 */
		static void CreateGPUMesh(Mesh& io_mesh);

		/**
		 * Add a task to the GPU load queue to create a GPU texture with
		 * OpenGL in the PollGPULoad() function.
		 *
		 * @param io_texture
		 */
		static void CreateGPUTexture(Texture& io_texture);

		/**
		* Get the default texture.
		*/
		static const DefaultTexture& GetDefaultTexture();

	private:
		ThreadPool tp;

		std::mutex resourceMX;
		std::mutex loadMX;

		std::mutex printMX;

		// Array of every loaded resource.
		std::unordered_map<std::string, std::shared_ptr<Resource>> resources;

		std::deque<Task> gpuLoadQueue;

		ResourcesManager() = default;

		/**
		 * Extrat Assimp's mesh data (vertices and textures) and load them into the given mesh.
		 * Try to load mesh's embeddeed textures if no specific texture file was specified.
		 *
		 * @param o_mesh
		 * @param i_aiMesh (from Assimp)
		 * @param i_aiScene
		 * @param i_textureFilename
		 * @param i_flipTexture
		 */
		void ProcessAiMesh(std::shared_ptr<Mesh>& o_mesh,
			const aiMesh& i_aim,
			const aiScene& i_scene,
			const std::string& i_textureFilename,
			const bool i_flipTexture);

		/**
		 * Go through every Assimp node to extract data (meshes) and add the loaded mesh into the given array of meshes.
		 * Tell ProcessAiMesh() to load embedded textures or not.
		 *
		 * @param o_meshes
		 * @param i_importer
		 * @param i_node
		 * @param i_textureFilename
		 * @param i_flipTexture
		 */
		void ProcessAiNode(std::list<std::shared_ptr<Mesh>>& o_meshes,
			const std::shared_ptr<Assimp::Importer> i_importer,
			const aiNode* i_node,
			const std::string& i_textureFilename,
			const bool i_flipTexture);

		/**
		 * Load a model from a file (group of meshes) into Assimp's importer.
		 *
		 * @param io_importer
		 * @param i_path
		 *
		 * @return Success
		 */
		int LoadCPUModel(Assimp::Importer& io_importer, const std::string& i_path);

		/**
		 * Reorder mesh's vertices with its stored indices.
		 *
		 * @param io_mesh
		 */
		void ParseMesh(Mesh& io_mesh);

		/**
		 * Load texture from Assimp's meshes.
		 *
		 * @param o_texture
		 * @param i_aiMaterial
		 * @param i_aiScene
		 * @param i_aiType
		 * @param i_type
		 */
		void LoadEmbeddedTexture(std::shared_ptr<Texture>& o_texture,
			const aiMaterial& i_mat,
			const aiScene& i_scene,
			const aiTextureType i_aiType,
			const ETextureType i_type);
	};
}

template<class TResource, typename... TExtraParams> requires std::constructible_from<TResource, const std::string, TExtraParams...>
std::shared_ptr<TResource> ResourcesManager::LoadResource(const std::string& i_name, const TExtraParams&... i_params) noexcept
{
	ResourcesManager& rm = ResourcesManager::Instance();

	std::lock_guard<std::mutex> guard(rm.resourceMX);

	if (rm.resources.find(i_name.c_str()) != rm.resources.end())
		return std::dynamic_pointer_cast<TResource>(rm.resources[i_name.c_str()]);

	rm.resources[i_name.c_str()] = std::make_shared<TResource>(i_name, i_params...);
	rm.resources[i_name.c_str()]->LoadFromInfo();

	return std::dynamic_pointer_cast<TResource>(rm.resources[i_name.c_str()]);
}