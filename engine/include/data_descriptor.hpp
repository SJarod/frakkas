#pragma once

#include <string>
#include <list>

/**
 * @brief Define a struct in-class to reflect field,
 * declare a static metaData variable and define the constructor of the class.
 */
#define KK_COMPONENT(compClass) \
\
static const char identifier_##compClass[] = #compClass;\
class compClass : public BaseComponent<identifier_##compClass>\
{\
private:\
\
	struct FieldMetaData\
	{\
		FieldMetaData(ClassMetaData& md, const char* n, DataType t, int c, size_t o)\
		{\
			md.descriptors.emplace_back(DataDescriptor(n, t, c, o));                          \
		}\
	};\
\
public: \
	static ClassMetaData metaData;\
	compClass() \
	{\
		metaData.className = this->GetID(); \
		metaData.classSize = sizeof(compClass);\
	}\
\
	const ClassMetaData& GetMetaData() const override { return metaData; };

#define KK_COMPONENT_END };

/**
 * @brief Define the static component's metadata in .cpp
 */
#define KK_COMPONENT_IMPL(compClass) \
	using namespace Game; \
    ClassMetaData compClass::metaData;

/**
 * @brief Add a FieldMetaData variable that will emplace back the reflected field in the static metadata in its constructor.
 */
#define KK_FIELD(fieldType, fieldName) \
    static FieldMetaData field_##fieldName; \
	fieldType fieldName

#define KK_FIELD_IMPL(compClass, fieldName, type, count) \
    compClass::FieldMetaData compClass::field_##fieldName{compClass::metaData, #fieldName, type, count, offsetof(compClass, fieldName)};

enum class DataType
{
	INT,
	FLOAT,
	BOOL,
	CAMERA,
    LIGHT
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

/**
 * @brief Store all the reflected field of a class
 */
struct ClassMetaData
{
	std::string className;
	size_t classSize = 0;
	std::list<DataDescriptor> descriptors;
};