#pragma once

#include <unordered_map>
#include <memory>

#include "utils/singleton.hpp"
#include "multithread/threadpool.hpp"

#include "resources/mesh.hpp"
#include "resources/texture.hpp"
#include "resources/skeletal_animation.hpp"

#include "resources/resource.hpp"

using namespace Resources;

namespace Resources
{
	class ResourcesManager : public Singleton<ResourcesManager>
	{
		friend class Singleton<ResourcesManager>;

	public:
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
		 * @param io_submesh
		 */
		static void CreateGPUSubmesh(Submesh& io_submesh);

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

		/**
		 * Get a reference to the resources map without options for changing it.
		 */
		static const std::unordered_map<std::string, std::shared_ptr<Resource>>& ViewAllResources();

	private:
		ThreadPool tp;

		std::mutex resourceMX;
		std::mutex loadMX;

		// Array of every loaded resource.
		std::unordered_map<std::string, std::shared_ptr<Resource>> resources;

		std::deque<Task> gpuLoadQueue;

		ResourcesManager() = default;
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
