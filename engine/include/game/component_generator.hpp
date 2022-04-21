#pragma once

#include "component.hpp"
#include "entity.hpp"
#include "data_descriptor.hpp"
#include "log.hpp"
//                componentGenerators[name] = std::make_unique<ComponentGenerator>(metaData, generator);\

/**
 * @brief Define a struct in-class to reflect field,
 * declare a static metaData variable and define the constructor of the class.
 */
#define KK_COMPONENT(compClass) \
                                \
class Entity;                                \
class compClass : public Component\
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
    struct ComponentRegister       \
    {                           \
        ComponentRegister()\
        {                       \
            compClass::metaData.className = #compClass;       \
            compClass::metaData.constructor = [](){ return new compClass(); };      \
            GetRegistry().push_back(&metaData);\
        }\
    };\
	static const ComponentRegister componentRegister;                           \
public: \
	static ClassMetaData metaData;\
\
	const ClassMetaData& GetMetaData() const override { return metaData; };      \
    const std::string& GetID() const override { return metaData.className; };\

#define KK_COMPONENT_END };

/**
 * @brief Define the static component's metadata in .cpp
 */
#define KK_COMPONENT_IMPL(compClass) \
	using namespace Game;               \
    ClassMetaData compClass::metaData; \
    const compClass::ComponentRegister compClass::componentRegister;                                     \


/**
 * @brief Add a FieldMetaData variable that will emplace back the reflected field in the static metadata in its constructor.
 */
#define KK_FIELD(fieldType, fieldName) \
    static FieldMetaData field_##fieldName; \
	fieldType fieldName

#define KK_FIELD_IMPL(compClass, fieldName, type, count) \
    compClass::FieldMetaData compClass::field_##fieldName{compClass::metaData, #fieldName, type, count, offsetof(compClass, fieldName)};
