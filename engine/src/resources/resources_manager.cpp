#include <stb_image.h>

#include <assimp/Importer.hpp>

#include "debug/log.hpp"
#include "maths.hpp"

#include "resources/sound.hpp"

#include "resources/resources_manager.hpp"

Resources::ResourcesManager::~ResourcesManager()
{
	// Has DestroyResources() been called?
}

const DefaultTexture& Resources::ResourcesManager::GetDefaultTexture()
{
	return Instance().defaultTexture;
}

const std::unordered_map<std::string, std::shared_ptr<Resource>>& Resources::ResourcesManager::ViewAllResources()
{
	return Instance().resources;
}

void Resources::ResourcesManager::Refresh()
{
	ResourcesManager& rm = Instance();

	for (auto& resource : rm.resources)
	{
		std::shared_ptr<Resource> ptr = resource.second;
		rm.threadpool.AddTask([&, ptr]() {
			ptr->CPUUnload();
			ptr->CPULoad();

			{
				std::lock_guard<std::mutex> guard(rm.gpuLoadMX);

				rm.gpuLoadQueue.emplace_back([ptr]() {
					ptr->GPUUnload();
					ptr->GPULoad();
					});
			}
			});
	}
}

void Resources::ResourcesManager::DestroyThisResource(const std::string& i_name)
{
	ResourcesManager& rm = Instance();
	std::lock_guard<std::mutex> guard(rm.resourceMX);

	std::string name = i_name;
	rm.resources.erase(name);

	Log::Info(name, " was deleted");
}

void Resources::ResourcesManager::DestroyResources()
{
	ResourcesManager& rm = Instance();

	while (!rm.threadpool.Clear())
	{
		// pausing main thread waiting for threads to finish CPU loading tasks
		std::this_thread::yield();
	}

	// finish remaining GPU loading tasks
	PollGPULoad();

	std::lock_guard<std::mutex> guard(rm.resourceMX);

	for (auto& resource : rm.resources)
	{
		resource.second->CPUUnload();
		resource.second->GPUUnload();
	}

	rm.resources.clear();
}

void Resources::ResourcesManager::PollGPULoad()
{
	ResourcesManager& rm = Instance();

	std::lock_guard<std::mutex> guard(rm.gpuLoadMX);

	for (Task& task : rm.gpuLoadQueue)
	{
		task();
	}

	rm.gpuLoadQueue.clear();
}