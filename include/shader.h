#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h> // gives us the requisite OpenGL headers

class Shader
{
    public:
        unsigned int ID;

        Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
        void use();
};

#endif
