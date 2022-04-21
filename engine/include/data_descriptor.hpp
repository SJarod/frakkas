#pragma once

#include <string>
#include <list>
#include <functional>
#include <unordered_map>

enum class DataType
{
	INT,
	FLOAT,
	BOOL,
	STRING,
	CAMERA,
    LIGHT,
    SOUND
};

/**
 * @brief Store informations about a class' field, to reflect class datas.
 */
struct DataDescriptor
{
	DataDescriptor(const std::string& name, DataType dataType, int count, size_t offset)
		:name(name), dataType(dataType), count(count), offset(offset){}

	std::string name;
	DataType dataType;
	int count = 1;
	size_t offset = 0;
};

namespace Game
{
    class Component;
}
/**
 * @brief Store all the reflected field of a class
 */
struct ClassMetaData
{
	std::string className;
    std::function<Game::Component*()> constructor; // [](){ return new PlayerComponent(); }
	std::list<DataDescriptor> descriptors;
};
