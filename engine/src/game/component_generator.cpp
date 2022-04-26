#include <string>
#include <sstream>
#include <algorithm>
#include <fstream>

#include "game/component_generator.hpp"

#ifdef CLION_IDE
static constexpr char gameDirectoryPath[] = "game/";
static constexpr char engineDirectoryPath[] = "engine/";
#else
static constexpr char gameDirectyoryPath[] = "../../game/";
static constexpr char engineDirectoryPath[] = "../../engine/";

#endif
static constexpr char lowComponentDirectoryPath[] = "game/lowcomponent/";
static constexpr char templateComponentName[] = "$name";
static constexpr char templateFileName[] = "$file";

inline std::string GetComponentFileName(const std::string& name)
{
	std::string fileName;
	for (int i = 0; i < name.size(); i++)
	{
		if (std::isupper(name[i]))
		{
			if (i != 0)
				fileName += '_';
			fileName += static_cast<char>(name[i] + 32);
		}
		else
			fileName += name[i];
	}

	return fileName;
}

inline std::string GetComponentSourceFullPath(const std::string& name)
{
	return gameDirectoryPath + std::string("src/") + name + ".cpp";
}

inline std::string GetComponentIncludeFullPath(const std::string& name)
{
	return gameDirectoryPath + std::string("include/game/") + name + ".hpp";
}

inline std::string GetCMakeFullPath()
{
	return gameDirectoryPath + std::string("CMakeLists.txt");
}

inline void StringReplaceAll(std::string& io_str, const std::string& i_from, const std::string& i_to)
{
    size_t lastFindPos = 0;
    while((lastFindPos = io_str.find(i_from, lastFindPos)) != io_str.npos)
    {
        io_str.replace(lastFindPos, i_from.length(), i_to);
        lastFindPos+= i_to.length(); // if i_to contains 'i_from', avoid finding it.
    }
}

bool CreateNewComponentScript(const std::string& compName)
{
	std::string fileName = GetComponentFileName(compName);
	std::ifstream checkFile(GetComponentIncludeFullPath(fileName));
	if (checkFile.is_open() || fileName == "drawable" || fileName == "camera_component" || fileName == "component")
	{
		checkFile.close();
		return false;
	}

    std::stringstream templateStream;
    std::string templateString;
	/// CREATE HEADER FILE
	std::ofstream newHeader(GetComponentIncludeFullPath(fileName));
    std::ifstream templateHeader(engineDirectoryPath + std::string("include/") + lowComponentDirectoryPath + std::string("component_template.hpp"));

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
    StringReplaceAll(templateString, templateComponentName, compName);

    // Write in new file
	newHeader << templateString;

	newHeader.close();
    std::stringstream().swap(templateStream); // clear stream

	/// CREATE SOURCE FILE
	std::ofstream newSource(GetComponentSourceFullPath(fileName));
    std::ifstream templateSource(engineDirectoryPath + std::string("src/") + lowComponentDirectoryPath + std::string("component_template.cpp"));
    // No need to check if it is opened, header checking is enough

    // Read template component header file in string
    templateStream << templateSource.rdbuf();
    templateString = templateStream.str();

    templateString.erase(0, templateString.find('\n')+1); // remove first line
    StringReplaceAll(templateString, templateComponentName, compName);
    StringReplaceAll(templateString, templateFileName, fileName);

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

	int endFirstLine = strCmake.find_first_of('\n');
	strCmake.insert(endFirstLine + 1, "\t\tsrc/" + fileName + ".cpp\n");

	std::ofstream outCMake(GetCMakeFullPath());

	outCMake << strCmake;
	outCMake.close();

	return true;
}
