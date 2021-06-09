#pragma once

#include <glad/glad.h>
#include <string>
#include <map>
#include "Shader.h"
#include "Texture2D.h"

class ResourceManager {
public:
    static std::map<std::string, Shader> Shaders;
    static std::map<std::string, Texture2D> Texture2Ds;
    static Shader LoadShader(const GLchar* vShaderFile, const GLchar* fShaderFile, const GLchar *gShaderFile, std::string name);
    static Shader &GetShader(std::string name);
    static Texture2D LoadTexture2D(const GLchar *file, GLboolean flip_vertically, std::string name);
    static Texture2D &GetTexture2D(std::string name);
    static void Clear();

private:
    ResourceManager();
    static Shader LoadShaderFromFile(const GLchar *vShaderFile, const GLchar *fShaderFile, const GLchar *gShaderFile = nullptr);
    static Texture2D LoadTexture2DFromFile(const GLchar *file, GLboolean flip_vertically = true);
};