#pragma once

#include <string>
#include <vector>
#include <functional>
#include <unordered_map>

#include "maths.hpp"

enum class EDataType
{
	INT,
	FLOAT,
	BOOL,
	STRING,
	CAMERA,
    SOUND
};

/**
 * @brief Store informations about a class' field, to reflect class datas.
 */
struct DataDescriptor
{
	DataDescriptor(const std::string& name, EDataType dataType, int count, size_t offset, bool viewOnly = false, const Vector2& range = Vector2())
		:name(name), dataType(dataType), count(count), offset(offset), viewOnly(viewOnly), range(range) {}

	std::string name;
	EDataType dataType;
	int count = 1;
	size_t offset = 0;
	bool viewOnly = false;
    Vector2 range;
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
    std::string parentClassName;
    bool publicClass = true;
    std::function<Game::Component*()> constructor; // [](){ return new PlayerComponent(); }
	std::vector<DataDescriptor> descriptors;
};
