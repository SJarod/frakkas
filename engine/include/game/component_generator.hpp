#pragma once

#include "debug/log.hpp"
#include "game/inputs_manager.hpp"
#include "game/time_manager.hpp"

#include "game/entity.hpp"
#include "game/lowcomponent/component.hpp"
#include "game/reflection/data_descriptor.hpp"

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
		FieldMetaData(ClassMetaData& md, const char* n, EDataType t, int c, size_t o)\
		{\
			md.descriptors.emplace_back(DataDescriptor(n, t, c, o)); \
		}\
	};\
                                \
                                \
    struct ComponentRegister       \
    {                           \
        ComponentRegister()\
        {                       \
            compClass::metaData.className = #compClass;       \
            compClass::metaData.constructor = [](){ return new compClass(); };\
            Log::Info("Register '", #compClass, "' component.");\
            GetRegistry().push_back(&metaData);\
        }\
    };\
	static const ComponentRegister componentRegister;                           \
public: \
	static ClassMetaData metaData;\
\
	const ClassMetaData& GetMetaData() const override { return metaData; };      \
    const std::string& GetID() const override { return metaData.className; };\

/**
 * Create a component inheriting from a parent component, may be an interface component.
 */
#define KK_COMPONENT_FROM(compClass, parentCompClass) \
                                \
class Entity;                                \
class compClass : public parentCompClass\
{\
private:\
\
    struct FieldMetaData\
    {\
        FieldMetaData(ClassMetaData& md, const char* n, EDataType t, int c, size_t o)\
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
            compClass::metaData.constructor = [](){ return new compClass(); };\
            Log::Info("Register '", #compClass, "' component.");\
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

/**
 * @brief Define a FieldMetaData that had been declare in the compClass.
 * @param compClass The component class name.
 * @param fieldName
 */
#define KK_FIELD_IMPL(compClass, fieldName, type, count) \
    _Pragma("GCC diagnostic push")                                                         \
    _Pragma("GCC diagnostic ignored \"-Winvalid-offsetof\"")                                                         \
    compClass::FieldMetaData compClass::field_##fieldName{compClass::metaData, #fieldName, type, count, offsetof(compClass, fieldName)};\
    _Pragma("GCC diagnostic pop")

/**
 * @brief Create new .cpp/.hpp component file if does not already exist.
 * @param i_compName name of the component class.
 * @return true if the component is added, false if not.
 */
bool CreateNewComponentScript(const std::string& i_compName);
