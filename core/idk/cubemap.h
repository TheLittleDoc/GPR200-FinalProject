#ifndef CUBEMAP_H
#define CUBEMAP_H

#include "../ew/external/glad.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <string>
#include <iostream>

namespace idk
{
    class Cubemap
    {
    public:
        Cubemap(std::vector<std::string> faces);
        ~Cubemap();
        unsigned int GetID();
    private:
        unsigned int m_id; //GL texture handle
    };
}

#endif