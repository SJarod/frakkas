//
// Created by f.marcellot on 21/03/2022.
//

#include "resources/serializer.hpp"

using namespace Resources;

///////////////////////// READ FUNCTIONS

std::string& Resources::Serializer::GetAttribute(std::ifstream &i_file, std::string &o_attribute) {
    std::getline(i_file, o_attribute);
    if (o_attribute.length() == 0)
    {
        std::cerr << "A scene attribute is not valid" << std::endl;
        return o_attribute;
    }
    o_attribute = o_attribute.substr(1);
    return o_attribute;
}

void Serializer::ReadVector3(std::ifstream& i_file, Vector3& o_vector) {
    char eatComma;
    i_file >> o_vector.element[0] >> eatComma;
    i_file >> o_vector.element[1] >> eatComma;
    i_file >> o_vector.element[2];
    i_file.ignore();
}

void Serializer::ReadString(std::ifstream& i_file, std::string& o_string) {
    i_file.ignore();
    std::getline(i_file, o_string);
}

///////////////////////// WRITE FUNCTIONS

void Serializer::WriteAttribute(std::ofstream &io_file, const std::string &i_attribute) {
    io_file << '>' << i_attribute << '\n';
}

char Serializer::Tab() {
    return '\t';
}

void Serializer::WriteVector3(std::ofstream &io_file, const Vector3& i_vector) {
    io_file << Tab() << i_vector.element[0] << ", " << i_vector.element[1] << ", " << i_vector.element[2] << std::endl;
}

void Serializer::WriteString(std::ofstream &io_file, const std::string &i_string) {
    io_file << Tab() << i_string << std::endl;
}


