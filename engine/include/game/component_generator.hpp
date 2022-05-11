#pragma once

#include <string>

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
		FieldMetaData(ClassMetaData& md, const char* n, EDataType t, int c, size_t o, bool vo = false, const Vector2& r = Vector2())\
		{                                                   \
			md.descriptors.emplace_back(DataDescriptor(n, t, c, o, vo, r)); \
		}\
	};\
                                \
    struct ComponentRegister       \
    {                           \
        ComponentRegister()\
        {                       \
            compClass::MetaData().className = #compClass;       \
            compClass::MetaData().constructor = [](){ return new compClass(); };\
            Log::Info("Register '", #compClass, "' component.");\
            GetRegistry().push_back(&MetaData());\
        }\
    };\
	static const ComponentRegister componentRegister;                           \
public: \
    static ClassMetaData& MetaData();           \
\
	const ClassMetaData& GetMetaData() const override { return MetaData(); };      \
    const std::string& GetID() const override { return MetaData().className; };

#define KK_PRIVATE_COMPONENT(compClass) \
\
class Entity;                                \
class compClass : public Component\
{\
private:\
\
	struct FieldMetaData\
	{\
		FieldMetaData(ClassMetaData& md, const char* n, EDataType t, int c, size_t o, bool vo = false, const Vector2& r = Vector2())\
		{                                                   \
			md.descriptors.emplace_back(DataDescriptor(n, t, c, o, vo, r)); \
		}\
	};\
                                \
    struct ComponentRegister       \
    {                           \
        ComponentRegister()\
        {                       \
            compClass::MetaData().className = #compClass;       \
            compClass::MetaData().constructor = [](){ return new compClass(); };                                              \
            compClass::MetaData().publicClass = false;\
            Log::Info("Register '", #compClass, "' component.");\
            GetRegistry().push_back(&MetaData());\
        }\
    };\
	static const ComponentRegister componentRegister;                           \
public: \
    static ClassMetaData& MetaData();           \
\
	const ClassMetaData& GetMetaData() const override { return MetaData(); };      \
    const std::string& GetID() const override { return MetaData().className; };

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
		FieldMetaData(ClassMetaData& md, const char* n, EDataType t, int c, size_t o, bool vo = false, const Vector2& r = Vector2())\
		{                                                   \
			md.descriptors.emplace_back(DataDescriptor(n, t, c, o, vo, r)); \
		}\
	};\
                                \
    struct ComponentRegister       \
    {                           \
        ComponentRegister()\
        {                       \
            compClass::MetaData().className = #compClass;                                                                     \
            compClass::MetaData().parentClassName = #parentCompClass;\
            compClass::MetaData().constructor = [](){ return new compClass(); };\
            GetRegistry().push_back(&MetaData());\
            Log::Info("Register '", #compClass, "' component.");\
        }\
    };\
    static const ComponentRegister componentRegister;                           \
public:                                               \
    static ClassMetaData& MetaData();           \
\
    const ClassMetaData& GetMetaData() const override { return MetaData(); };      \
    const std::string& GetID() const override { return MetaData().className; };

#define KK_COMPONENT_END };

/**
 * @brief Define the static component's metadata and componentRegister in .cpp
 */
#define KK_COMPONENT_IMPL(compClass) \
	using namespace Game;               \
    const compClass::ComponentRegister compClass::componentRegister;\
    ClassMetaData& compClass::MetaData()\
    {                                \
        static ClassMetaData metaData; \
        return metaData; \
    }

/**
 * @brief Add a FieldMetaData variable that will emplace back the reflected field in the static metadata in its constructor.
 */
#define KK_FIELD(fieldType, fieldName) \
    static FieldMetaData field_##fieldName; \
	fieldType fieldName

/**
 * @brief Define a FieldMetaData that had been declare in the compClass.
 * @param compClass The component class name.
 * @param fieldName Name of the field.
 * @param type an EDataType parameter to specify the kind of the data.
 * @param count The count of EDataType element, so that you can define arrays.
 */
#define KK_FIELD_VECTOR_IMPL(compClass, fieldName, type, count) \
    _Pragma("GCC diagnostic push")                                                         \
    _Pragma("GCC diagnostic ignored \"-Winvalid-offsetof\"")                                                         \
    compClass::FieldMetaData compClass::field_##fieldName{compClass::MetaData(), #fieldName, type, count, offsetof(compClass, fieldName)};\
    _Pragma("GCC diagnostic pop")

/**
 * @brief Define a FieldMetaData that had been declare in the compClass, with only 1 scalar.
 * @param compClass The component class name.
 * @param fieldName Name of the field.
 * @param type an EDataType parameter to specify the kind of the data.
 */
#define KK_FIELD_IMPL(compClass, fieldName, type) \
    _Pragma("GCC diagnostic push")                                                         \
    _Pragma("GCC diagnostic ignored \"-Winvalid-offsetof\"")                                                         \
    compClass::FieldMetaData compClass::field_##fieldName{compClass::MetaData(), #fieldName, type, 1, offsetof(compClass, fieldName)};\
    _Pragma("GCC diagnostic pop")

/**
 * @brief Define a FieldMetaData that had been declare in the compClass. Not serialize and not edited, just print in the inspector.
 * @param compClass The component class name.
 * @param fieldName Name of the field.
 * @param type an EDataType parameter to specify the kind of the data.
 * @param count The count of EDataType element, so that you can define arrays.
 */
#define KK_FIELD_VIEW_ONLY_IMPL(compClass, fieldName, type, count) \
    _Pragma("GCC diagnostic push")                                                         \
    _Pragma("GCC diagnostic ignored \"-Winvalid-offsetof\"")                                                         \
    compClass::FieldMetaData compClass::field_##fieldName{ compClass::MetaData(), #fieldName, type, count, offsetof(compClass, fieldName), true }; \
    _Pragma("GCC diagnostic pop")
/**
 * @brief Define a FieldMetaData that had been declare in the compClass.
 * @param compClass The component class name.
 * @param fieldName Name of the field.
 * @param type an EDataType parameter to specify the kind of the data.
 * @param count The count of EDataType element, so that you can define arrays.
 * @param valueMin The minimum float value to clamp the field.
 * @param valueMax The maximum float value to clamp the field.
 */
#define KK_FIELD_RANGE_IMPL(compClass, fieldName, type, count, valueMin, valueMax)\
    _Pragma("GCC diagnostic push")                                                         \
    _Pragma("GCC diagnostic ignored \"-Winvalid-offsetof\"")                                                         \
    compClass::FieldMetaData compClass::field_##fieldName{ compClass::MetaData(), #fieldName, type, count, offsetof(compClass, fieldName), false, Vector2(valueMin, valueMax) }; \
    _Pragma("GCC diagnostic pop")



/**
 * @brief Create new .cpp/.hpp component file if does not already exist.
 * @param i_compName name of the component class.
 * @return true if the component is added, false if not.
 */
bool CreateNewComponentScript(const std::string& i_compName);
