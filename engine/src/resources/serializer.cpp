#include "debug/log.hpp"
#include "game/reflection/data_descriptor.hpp"

#include "game/transform.hpp"
#include "game/entity.hpp"
#include "game/lowcomponent/component.hpp"
#include "game/entity_manager.hpp"

#include "resources/sound.hpp"

#include "renderer/light.hpp"
#include "renderer/model.hpp"
#include "renderer/skeletal_model.hpp"

#include "resources/mesh.hpp"
#include "resources/skeletal_mesh.hpp"
#include "resources/skeletal_animation.hpp"
#include "resources/serializer.hpp"

using namespace Resources;

std::string Serializer::attribute;

///////////////////////// READ FUNCTIONS

std::string& Resources::Serializer::GetAttribute(std::ifstream& i_file)
{
    if (i_file.eof())
    {
        attribute = "End of file";
        return attribute;
    }

    std::getline(i_file, attribute);

    if (attribute.empty() || attribute[0] != '>')
        return GetAttribute(i_file); // Call Get attribute until we find an attribute.


    attribute = attribute.substr(1);
    return attribute;
}

void Serializer::Read(std::ifstream& i_file, Game::Entity& o_entity)
{
    int componentCount = 0;
    GetAttribute(i_file);
    Read(i_file, &componentCount, 1);

    GetAttribute(i_file);
    Read(i_file, o_entity.transform);

    for (int i = 0; i < componentCount; i++)
    {
        GetAttribute(i_file); // name of component

        auto& registry = Game::Component::GetRegistry();
        auto it = std::find_if(registry.begin(), registry.end(), [](ClassMetaData* md){return attribute == md->className;});
        if (it == registry.end())
        {
            Log::Error("Can't read " + attribute + " component's data.");
            break;
        }

        Game::Component* comp = o_entity.AddComponent(std::unique_ptr<Game::Component>((*it)->constructor()));
        bool compEnabled = comp->enabled;
        Read(i_file, reinterpret_cast<unsigned char*>(comp), comp->GetMetaData(), compEnabled);
        comp->enabled = compEnabled;
    }
}

void Serializer::CreateAndReadEntity(std::ifstream& i_file, Game::EntityManager& io_entityManager, Game::Entity* i_parent)
{
    int childCount = 0;
    GetAttribute(i_file); // '>entity'
    if (attribute != "entity")
        return;

    GetAttribute(i_file); // '>childs'
    if (attribute != "childs")
        return;
    Read(i_file, &childCount, 1);

    std::string entityName;
    GetAttribute(i_file); // '>name'
    if (attribute != "name")
        return;
    Read(i_file, entityName);

    Game::Entity* entity = io_entityManager.CreateEntity(entityName);

    if (i_parent)
        io_entityManager.SetEntityParent(*entity, *i_parent);

    Read(i_file, *entity); // Read components

    i_file.ignore(); // skip lines

    // Load child which are the next written entities, and set this entity as parent.
    for (int i = 0; i < childCount; i++)
        CreateAndReadEntity(i_file, io_entityManager, entity);
}

void Serializer::Read(std::ifstream& i_file, unsigned char* o_component, const ClassMetaData& i_metaData, bool& o_enabled)
{
    GetAttribute(i_file);
    Read(i_file, o_enabled);
    Read(i_file, o_component, i_metaData);
}

void Serializer::Read(std::ifstream& i_file, unsigned char* o_component, const ClassMetaData& i_metaData)
{
    if (!i_metaData.parentClassName.empty())
        Read(i_file, o_component, *Game::Component::FindMetaData(i_metaData.parentClassName));

    for (const DataDescriptor& desc : i_metaData.descriptors)
    {
        if (desc.viewOnly) continue;

        unsigned char* componentData = o_component + desc.offset;

        GetAttribute(i_file);
        if (attribute == desc.name)
        {
            switch (desc.dataType)
            {
                case EDataType::BOOL:
                    Read(i_file, *reinterpret_cast<bool*>(componentData));
                    break;
                case EDataType::INT:
                    Read(i_file, reinterpret_cast<int*>(componentData), desc.count);
                    break;
                case EDataType::FLOAT:
                    Read(i_file, reinterpret_cast<float*>(componentData), desc.count);
                    break;
                case EDataType::STRING:
                    Read(i_file, *reinterpret_cast<std::string*>(componentData));
                    break;
                case EDataType::SOUND:
                    Read(i_file, *reinterpret_cast<Resources::Sound*>(componentData));
                    break;
                case EDataType::MODEL:
                    Read(i_file, *reinterpret_cast<Renderer::Model*>(componentData));
                    break;
                case EDataType::SKELETALMODEL:
                    Read(i_file, *reinterpret_cast<Renderer::SkeletalModel*>(componentData));
                    break;
                default:
                    break;
            }
        }
    }

}

void Serializer::Read(std::ifstream& i_file, std::string& o_string)
{
    i_file.ignore();
    std::getline(i_file, o_string);
}

void Serializer::Read(std::ifstream& i_file, bool& o_value)
{
    int val = 0;
    Read(i_file, &val, 1);
    o_value = static_cast<bool>(val);
}

void Serializer::Read(std::ifstream& i_file, Game::Transform& o_transform)
{
    for(int i = 0; i < 3; i++)
    {
        GetAttribute(i_file);
        if (attribute == "position")
        {
            Vector3 position = o_transform.position;
            Read(i_file, position.element, 3);
            o_transform.position = position;
        }
        else if (attribute == "rotation")
        {
            Vector3 rotation = o_transform.rotation;
            Read(i_file, rotation.element, 3);
            o_transform.rotation = rotation;
        }
        else if (attribute == "scale")
        {
            Vector3 scale = o_transform.scale;
            Read(i_file, scale.element, 3);
            o_transform.scale = scale;
        }
    }
}

void Serializer::Read(std::ifstream& i_file, Renderer::Light& o_light)
{
    for (int i = 0; i < 11; i++)
    {
        GetAttribute(i_file);
        if (attribute == "position")
            Read(i_file, o_light.position.element, 4);
        else if (attribute == "ambient")
            Read(i_file, o_light.ambient.element, 3);
        else if (attribute == "diffuse")
            Read(i_file, o_light.diffuse.element, 3);
        else if (attribute == "specular")
            Read(i_file, o_light.specular.element, 3);
        else if (attribute == "toonShading")
            Read(i_file, o_light.toonShading);
        else if (attribute == "stepAmount")
            Read(i_file, &o_light.stepAmount, 1);
        else if (attribute == "stepSize")
            Read(i_file, &o_light.stepSize, 1);
        else if (attribute == "specSize")
            Read(i_file, &o_light.specSize, 1);
        else if (attribute == "shadow")
            Read(i_file, o_light.shadow);
        else if (attribute == "adaptativeBias")
            Read(i_file, o_light.adaptativeBias);
        else if (attribute == "shadowBias")
            Read(i_file, &o_light.shadowBias, 1);
    }
}

void Serializer::Read(std::ifstream& i_file, Sound& o_sound)
{
    for (int i = 0; i < 2; ++i)
    {
        GetAttribute(i_file);
        if(attribute == "soundPath")
            Read(i_file, o_sound.soundPath);
        else if (attribute == "volume")
            Read(i_file, &o_sound.volume);
    }

    o_sound.SetSound(o_sound.soundPath);
    o_sound.SetVolume();
}

void Serializer::Read(std::ifstream& i_file, Renderer::Model& o_model)
{
    std::string meshPath, texturePath;
    for (int i = 0; i < 2; ++i)
    {
        GetAttribute(i_file);
        if (attribute == "name")
            Read(i_file, meshPath);
        else if (attribute == "textureName")
            Read(i_file, texturePath);
    }

    if (meshPath != "none")
    {
        o_model.SetMeshFromFile(meshPath);
        if (texturePath != "none")
            o_model.SetTextureToAllSubmesh(texturePath, false);
    }
}

void Serializer::Read(std::ifstream& i_file, Renderer::SkeletalModel& o_skmodel)
{
    std::string meshPath, texturePath, animationPath;
    for (int i = 0; i < 3; ++i)
    {
        GetAttribute(i_file);
        if (attribute == "name")
            Read(i_file, meshPath);
        else if (attribute == "textureName")
            Read(i_file, texturePath);
        else if (attribute == "animationFilename")
            Read(i_file, animationPath);
    }

    if (meshPath != "none")
    {
        o_skmodel.SetSkeletalMeshFromFile(meshPath);
        if (texturePath != "none")
            o_skmodel.SetTextureToAllSubmesh(texturePath, false);
        if (animationPath != "none")
            o_skmodel.LoadAnimationsForThis(animationPath);
    }
}

///////////////////////// WRITE FUNCTIONS

void Serializer::WriteAttribute(std::ofstream &io_file, const std::string &i_attribute)
{
    io_file << '>' << i_attribute << std::endl;
}

void Serializer::Write(std::ofstream& io_file, const Game::Entity& i_entity)
{
    io_file << '\n';

    WriteAttribute(io_file, "entity");

    int childCount = i_entity.childs.size();
    Write(io_file, "childs", &childCount, 1);

    Write(io_file, "name", i_entity.name);

    int compCount = static_cast<int>(i_entity.components.size());
    Write(io_file, "component_count", &compCount, 1);

    Write(io_file, "transform", i_entity.transform);

    for(const std::unique_ptr<Game::Component>& comp : i_entity.components)
        Write(io_file, reinterpret_cast<unsigned char*>(comp.get()), comp->GetMetaData(), comp->enabled.get());

    for (const Game::Entity* child : i_entity.childs)
        Write(io_file, *child);
}

void Serializer::Write(std::ofstream& io_file, unsigned char* i_component, const ClassMetaData& i_metaData, bool i_enabled)
{
    WriteAttribute(io_file, i_metaData.className);
    Write(io_file, "enabled", i_enabled);
    Write(io_file, i_component, i_metaData);
}

void Serializer::Write(std::ofstream& io_file, unsigned char* i_component, const ClassMetaData& i_metaData)
{
    if(!i_metaData.parentClassName.empty())
        Write(io_file, i_component, *Game::Component::FindMetaData(i_metaData.parentClassName));

    for (const DataDescriptor& desc : i_metaData.descriptors)
    {
        if (desc.viewOnly) continue;

        unsigned char* componentData = i_component + desc.offset;

        switch (desc.dataType)
        {
            case EDataType::BOOL:
                Write(io_file, desc.name, *reinterpret_cast<bool*>(componentData));
                break;
            case EDataType::INT:
                Write(io_file, desc.name, reinterpret_cast<int*>(componentData), desc.count);
                break;
            case EDataType::FLOAT:
                Write(io_file, desc.name, reinterpret_cast<float*>(componentData), desc.count);
                break;
            case EDataType::STRING:
                Write(io_file, desc.name, *reinterpret_cast<std::string*>(componentData));
                break;
            case EDataType::SOUND:
                Write(io_file, desc.name, *reinterpret_cast<Resources::Sound*>(componentData));
                break;
            case EDataType::MODEL:
                Write(io_file, desc.name, *reinterpret_cast<Renderer::Model*>(componentData));
                break;
            case EDataType::SKELETALMODEL:
                Write(io_file, desc.name, *reinterpret_cast<Renderer::SkeletalModel*>(componentData));
                break;
            default:
                break;
        }
    }

}

char Serializer::Tab()
{
    return '\t';
}

unsigned int Resources::Serializer::GetCurrentLine(std::ifstream& i_file)
{
    auto originalPos = i_file.tellg();

    i_file.seekg(0);

    int count = 0;
    std::string line;
    // Read line one by one, and check cursor position
    // 'for' loop stop when cursor moves beyond the original position
    for (; i_file.tellg() < originalPos; count++)
    {
        std::getline(i_file, line);
        std::cout << static_cast<int>(i_file.tellg()) << std::endl;
    }

    i_file.seekg(originalPos);

    return count;
}

void Serializer::Write(std::ofstream& io_file, const std::string& i_attributeName, bool i_bool)
{
    int i = i_bool ? 1 : 0;
    Write(io_file, i_attributeName, &i, 1);
}

void Serializer::Write(std::ofstream& io_file, const std::string& i_attributeName, const std::string& i_string)
{
    WriteAttribute(io_file, i_attributeName);
    io_file << Tab() << i_string << std::endl;
}

void Serializer::Write(std::ofstream& io_file, const std::string& i_attributeName, const Game::Transform& i_transform)
{
    WriteAttribute(io_file, i_attributeName);
    Write(io_file, "position", i_transform.position.get().element, 3);
    Write(io_file, "rotation", i_transform.rotation.get().element, 3);
    Write(io_file, "scale", i_transform.scale.get().element, 3);
}

void Serializer::Write(std::ofstream& io_file, const std::string& i_attributeName, const Renderer::Light& i_light)
{
    WriteAttribute(io_file, i_attributeName);
    Write(io_file, "position", i_light.position.element, 4);
    Write(io_file, "ambient", i_light.ambient.element, 3);
    Write(io_file, "diffuse", i_light.diffuse.element, 3);
    Write(io_file, "specular", i_light.specular.element, 3);
    Write(io_file, "toonShading", &i_light.toonShading);
    Write(io_file, "stepAmount", &i_light.stepAmount);
    Write(io_file, "stepSize", &i_light.stepSize);
    Write(io_file, "specSize", &i_light.specSize);
    Write(io_file, "shadow", &i_light.shadow);
    Write(io_file, "adapativeBias", &i_light.adaptativeBias);
    Write(io_file, "shadowBias", &i_light.shadowBias);
}

void Serializer::Write(std::ofstream& io_file, const std::string& i_attributeName, const Sound& i_sound)
{
    WriteAttribute(io_file, i_attributeName);
    Write(io_file, "soundPath", i_sound.soundPath);
    Write(io_file, "volume", &i_sound.volume);
}

void Serializer::Write(std::ofstream& io_file, const std::string& i_attributeName, const Renderer::Model& i_model)
{
    bool meshExists = i_model.mesh ? true : false;
    WriteAttribute(io_file, i_attributeName);
    Write(io_file, "name", meshExists ? i_model.mesh->name : "none");
    Write(io_file, "textureName", meshExists ? i_model.mesh->textureName : "none");
}

void Serializer::Write(std::ofstream& io_file, const std::string& i_attributeName, const Renderer::SkeletalModel& i_skmodel)
{
    bool meshExists = i_skmodel.skmesh ? true : false;
    WriteAttribute(io_file, i_attributeName);
    Write(io_file, "name", meshExists ? i_skmodel.skmesh->name : "none");
    Write(io_file, "textureName", meshExists ? i_skmodel.skmesh->textureName : "none");
    Write(io_file, "animationFilename", i_skmodel.skpack ? i_skmodel.skpack->animationFilename : "none");
}
