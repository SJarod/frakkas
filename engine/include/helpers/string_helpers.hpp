#pragma once

#include <string>

namespace Helpers
{

    /**
     * @brief Replace all matching string by another string.
     * @param io_str The full string to modify.
     * @param i_from The string to replace.
     * @param i_to The new string to set.
     */
    inline void StringReplaceAll(std::string& io_str, const std::string& i_from, const std::string& i_to)
    {
        size_t lastFindPos = 0;
        while((lastFindPos = io_str.find(i_from, lastFindPos)) != io_str.npos)
        {
            io_str.replace(lastFindPos, i_from.length(), i_to);
            lastFindPos+= i_to.length(); // if i_to contains 'i_from', avoid finding it.
        }
    }

    /**
     * Check if a string respect the CamelCase convention.
     * @param i_str The string to check.
     * @return true if it is camel case, else return false.
     */
    inline bool IsCamelCase(const std::string& i_str)
    {
        int index = 0;
        for(char c : i_str)
        {
            if ('\0' == c)
                break;
            if (index == 0 && !std::isupper(c)) // return false if first letter not upper
                return false;
            if (!std::isalpha(c)) // return false if we have special character.
                return false;

            index++;
        }

        return true;
    }

    /**
     * Modify a CamelCase string into snake_case string.
     * If the sent string is not camel case, we return it with any modifications.
     * @param i_camelStr A CamelCase string.
     * @return A snake_case string.
     */
    inline std::string CamelCaseToSnakeCase(const std::string& i_camelStr)
    {
        if (!IsCamelCase(i_camelStr)) return i_camelStr;

        std::string snakeStr;
        for (int i = 0; i < i_camelStr.size(); i++)
        {
            if (std::isupper(i_camelStr[i]))
            {
                if (i != 0)
                    snakeStr += '_';
                snakeStr += static_cast<char>(i_camelStr[i] + 32);
            }
            else
                snakeStr += i_camelStr[i];
        }

        return snakeStr;
    }
}
