//
// Created by f.marcellot on 21/03/2022.
//

#pragma once

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "maths.hpp"

namespace Resources
{
    class Serializer
    {
    public: /// READ FUNCTIONS
        /**
         *
         * @param i_file the opened input file to read line
         * @param o_attribute the output attribute's name
         * @return the output attribute's name. So that we can use this function in condition structure.
         */
        std::string& GetAttribute(std::ifstream& i_file, std::string& o_attribute);

        /**
         * @brief read a Vector3 in the next line of i_file
         * @param i_file the opened input file
         * @param o_vector the Vector3 to build
         */
        void ReadVector3(std::ifstream& i_file, Vector3& o_vector);
        /**
         * @brief read a string in the next line of i_file
         * @param i_file the opened input file
         * @param o_string the string to build
         */
        void ReadString(std::ifstream& i_file, std::string& o_string);


    public: /// WRITE FUNCTIONS
        /**
         * @brief Write an attribute in frakkas text format
         * @param io_file the opened output file to write in
         * @param i_attribute the variable type to write
         */
        void WriteAttribute(std::ofstream& io_file, const std::string& i_attribute);

        /**
         * @brief Write a Vector3 in frakkas text format
         * @param io_file the opened output file to write in
         * @param i_vector the Vector3 to write
         */
        void WriteVector3(std::ofstream& io_file, const Vector3& i_vector);
        /**
         * @brief Write a string in frakkas text format
         * @param io_file the opened output file to write in
         * @param i_vector the string to write
         */
        void WriteString(std::ofstream& io_file, const std::string& i_string);

    private:

        /**
         * @return '\t', so a tabulation in character format. Useful to write tabulation with std::cout
         */
        char Tab();
    };
}
