//
// Created by f.marcellot on 21/03/2022.
//

#include "engine/serializer.hpp"

using namespace Engine;

///////////////////////// READ FUNCTIONS

std::string& Engine::Serializer::GetAttribute(std::ifstream &i_file, std::string &o_attribute) {
    std::getline(i_file, o_attribute);
    if (o_attribute.length() == 0)
    {
        std::cerr << "A scene attribute is not valid" << std::endl;
        return o_attribute;
    }
    o_attribute = o_attribute.substr(1);
    return o_attribute;
}

void Engine::Serializer::ReadScene(const std::string &i_filePath, Scene& o_scene) {
    std::ifstream inFile(i_filePath);

    if (inFile.fail())
    {
        std::cerr << "Input file not found : " << i_filePath << std::endl;
        return;
    }

    std::getline(inFile, o_scene.sceneName);

    std::string inAttribute;
    GetAttribute(inFile, inAttribute);
    while (!inFile.eof())
    {
        if (inAttribute == "exemple")
        {
            inAttribute = ReadExemple(inFile, o_scene.exemples[o_scene.count++]);
            continue;
        }
        GetAttribute(inFile, inAttribute);
    }

    inFile.close();
}

std::string Engine::Serializer::ReadExemple(std::ifstream &i_file, Exemple& o_exemple) {

    std::string attribute;
    while(!i_file.eof())
    {
        GetAttribute(i_file, attribute);

        if (attribute == "position")
            ReadVector3(i_file, o_exemple.xyz);
        else if (attribute == "str")
            ReadString(i_file, o_exemple.str);
        else
            break;
    }

    return attribute;
}

void Serializer::ReadVector3(std::ifstream& i_file, float* o_vector) {
    char eatComma;
    i_file >> o_vector[0] >> eatComma;
    i_file >> o_vector[1] >> eatComma;
    i_file >> o_vector[2];
    i_file.ignore();
}

void Serializer::ReadString(std::ifstream& i_file, std::string& o_string) {
    i_file.ignore();
    std::getline(i_file, o_string);
}

///////////////////////// WRITE FUNCTIONS

void Engine::Serializer::WriteScene(const std::string &i_filePath, const Scene &i_scene) {
    std::ofstream outFile(i_filePath);
    if (outFile.fail())
    {
        std::cerr << "Output file impossible to open : " << i_filePath << std::endl;
        return;
    }

    outFile << i_scene.sceneName << '\n';

    for (int i = 0; i < i_scene.count; i++)
        WriteExemple(outFile, i_scene.exemples[i]);

    outFile.close();
}

void Engine::Serializer::WriteExemple(std::ofstream &io_file, const Exemple &i_exemple) {
    WriteAttribute(io_file, "exemple");

    WriteAttribute(io_file, "position");
    WriteVector3(io_file, i_exemple.xyz);

    WriteAttribute(io_file, "str");
    WriteString(io_file, i_exemple.str);
}

void Serializer::WriteAttribute(std::ofstream &io_file, const std::string &i_attribute) {
    io_file << '>' << i_attribute << '\n';
}

char Serializer::Tab() {
    return '\t';
}

void Serializer::WriteVector3(std::ofstream &io_file, const float *i_vector) {
    io_file << Tab() << i_vector[0] << ", " << i_vector[1] << ", " << i_vector[2] << std::endl;
}

void Serializer::WriteString(std::ofstream &io_file, const std::string &i_string) {
    io_file << Tab() << i_string << std::endl;
}


