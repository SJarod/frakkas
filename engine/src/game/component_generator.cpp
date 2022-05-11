#include <string>

#include "helpers/string_helpers.hpp"
#include "helpers/path_constants.hpp"

#include "game/component_generator.hpp"

static constexpr char lowComponentDirectoryPath[] = "game/lowcomponent/";
static constexpr char cmakeAddLibraryString[] = "add_library(GameFrakkas";
static constexpr char templateComponentName[] = "$name";
static constexpr char templateFileName[] = "$file";


inline std::string GetComponentSourceFullPath(const std::string& name)
{
    return Helpers::gameDirectoryPath + std::string("src/game/") + name + ".cpp";
}

inline std::string GetComponentIncludeFullPath(const std::string& name)
{
	return Helpers::gameDirectoryPath + std::string("include/game/") + name + ".hpp";
}

inline std::string GetCMakeFullPath()
{
	return Helpers::gameDirectoryPath + std::string("CMakeLists.txt");
}

bool CreateNewComponentScript(const std::string& compName)
{
	std::string fileName = Helpers::CamelCaseToSnakeCase(compName);

	std::ifstream checkFile(GetComponentIncludeFullPath(fileName));
    std::string lowComponentList = "drawable camera_component component";
	if (checkFile.is_open() || lowComponentList.find(fileName) != std::string::npos)
	{
		checkFile.close();
		return false;
	}

    std::stringstream templateStream;
    std::string templateString;
	/// CREATE HEADER FILE
	std::ofstream newHeader(GetComponentIncludeFullPath(fileName));
    std::ifstream templateHeader(Helpers::engineDirectoryPath + std::string("include/") + lowComponentDirectoryPath + std::string("component_template.hpp"));

    if (!newHeader.is_open())
    {
        Log::Warning("Could not open a new header '", GetComponentIncludeFullPath(fileName), "' for component '", compName, "'.");
        return false;
    }

    if (!templateHeader.is_open())
    {
        Log::Warning("Could not open the template Header 'engine/include/game/lowcomponent/template_component.hpp'.");
        return false;
    }

    // Read template component header file in string
    templateStream << templateHeader.rdbuf();
    templateString = templateStream.str();

    templateString.erase(0, templateString.find('\n')+1); // remove first line
    Helpers::StringReplaceAll(templateString, templateComponentName, compName);

    // Write in new file
	newHeader << templateString;

	newHeader.close();
    std::stringstream().swap(templateStream); // clear stream

	/// CREATE SOURCE FILE
	std::ofstream newSource(GetComponentSourceFullPath(fileName));
    std::ifstream templateSource(Helpers::engineDirectoryPath + std::string("src/") + lowComponentDirectoryPath + std::string("component_template.cpp"));
    // No need to check if it is opened, header checking is enough

    // Read template component header file in string
    templateStream << templateSource.rdbuf();
    templateString = templateStream.str();

    templateString.erase(0, templateString.find('\n')+1); // remove first line
    Helpers::StringReplaceAll(templateString, templateComponentName, compName);
    Helpers::StringReplaceAll(templateString, templateFileName, fileName);

    // Write in new file
    newSource << templateString;

	newSource.close();

	///  UPDATE CMAKE
	std::string strCmake;
	std::ifstream inCMake(GetCMakeFullPath());
	std::stringstream stream;

	stream << inCMake.rdbuf();
	inCMake.close();

	strCmake = stream.str();

	int addLibraryLine = strCmake.find(cmakeAddLibraryString) + (sizeof(cmakeAddLibraryString) / sizeof(char));
	strCmake.insert(addLibraryLine + 1, "\t\tsrc/game/" + fileName + ".cpp\n");

	std::ofstream outCMake(GetCMakeFullPath());

	outCMake << strCmake;
	outCMake.close();

    /// UPDATE COMPONENT REGISTER
    std::string registerPath = Helpers::gameDirectoryPath + std::string("src/") + lowComponentDirectoryPath + std::string("component_register.cpp");
    std::ifstream inRegisterSource(registerPath);

    if (!inRegisterSource.is_open())
    {
        Log::Warning("Could not open component register file \"game/src/game/lowcomponent/component_register.cpp\"");
        return false;
    }

    // read file
    std::stringstream registerStream;
    registerStream << inRegisterSource.rdbuf();
    std::string registerString(registerStream.str());

    registerString.insert(0, "#include \"game/" + fileName + ".hpp\"\n");
    registerString.insert(registerString.find('$')+2, "\tgreedEntity.AddComponent<" + compName + ">();\n");

    inRegisterSource.close();

    // write file
    std::ofstream outRegisterSource(registerPath);
    outRegisterSource << registerString;

    outRegisterSource.close();

    return true;
}
