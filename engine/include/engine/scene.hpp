//
// Created by f.marcellot on 21/03/2022.
//

#pragma once  

#include <string>
#include <vector>

struct Exemple
{
    float xyz[3];

    std::string str;
};

struct Scene
{
    std::string sceneName;
    Exemple* exemples;
    int count = 0;
};