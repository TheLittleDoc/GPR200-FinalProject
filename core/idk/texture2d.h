#ifndef TEXTURE2D_H
#define TEXTURE2D_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

namespace idk
{
    class Texture2D {
    public:
        Texture2D(const char* filePath, int filterMode, int wrapMode, bool transparent = false);
        ~Texture2D();
        void Bind(unsigned int slot = 0); //Bind to a specific texture unit
        unsigned int GetID();
    private:
        unsigned int m_id; //GL texture handle
        int m_width, m_height;
    };
}

#endif