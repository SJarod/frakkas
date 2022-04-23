#pragma once

#include <functional>
/*
struct Task
{
	void* data;
};

struct ResourceLoadTaskData
{
	std::vector<Mesh> meshes;
};
*/
using Task = std::function<void()>;