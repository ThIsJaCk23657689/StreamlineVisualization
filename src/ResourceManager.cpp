#include "ResourceManager.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <stb_image.h>

std::map<std::string, Shader> ResourceManager::Shaders;
std::map<std::string, Texture2D> ResourceManager::Texture2Ds;

Shader ResourceManager::LoadShader(const GLchar *vShaderFile, const GLchar *fShaderFile, const GLchar *gShaderFile,
                                   std::string name) {
    Shaders[name] = LoadShaderFromFile(vShaderFile, fShaderFile, gShaderFile);
    return Shaders[name];
}

Shader &ResourceManager::GetShader(std::string name) {
    return Shaders[name];
}

Texture2D ResourceManager::LoadTexture2D(const GLchar *file, GLboolean flip_vertically, std::string name) {
    Texture2Ds[name] = LoadTexture2DFromFile(file, flip_vertically);
    return Texture2Ds[name];
}

Texture2D &ResourceManager::GetTexture2D(std::string name) {
    return Texture2Ds[name];
}

void ResourceManager::Clear() {
    for (auto iter : Shaders) {
        glDeleteProgram(iter.second.ID);
    }
    for (auto iter : Texture2Ds) {
        glDeleteTextures(1, &iter.second.ID);
    }
}

Shader ResourceManager::LoadShaderFromFile(const GLchar *vShaderFile, const GLchar *fShaderFile,
                                           const GLchar *gShaderFile) {
    std::string vertexCode;
    std::string fragmentCode;
    std::string geometryCode;

    try {
        std::ifstream vertexShaderFile(vShaderFile);
        std::ifstream fragmentShaderFile(fShaderFile);
        std::stringstream vShaderStream, fShaderStream;

        vShaderStream << vertexShaderFile.rdbuf();
        fShaderStream << fragmentShaderFile.rdbuf();
        vertexShaderFile.close();
        fragmentShaderFile.close();

        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();

        if (gShaderFile != nullptr) {
            std::ifstream geometryShaderFile(geometryCode);
            std::stringstream gShaderStream;
            gShaderStream << geometryShaderFile.rdbuf();
            geometryShaderFile.close();
            geometryCode = gShaderStream.str();
        }
    } catch (std::exception e) {
        std::cout << "ERROR::SHADER: Failed to read shader files" << std::endl;
    }

    const GLchar *vShaderCode = vertexCode.c_str();
    const GLchar *fShaderCode = fragmentCode.c_str();
    const GLchar *gShaderCode = geometryCode.c_str();

    Shader shader;
    shader.Compile(vShaderCode, fShaderCode, gShaderFile != nullptr ? gShaderCode : nullptr);
    return shader;
}

Texture2D ResourceManager::LoadTexture2DFromFile(const GLchar *file, GLboolean flip_vertically) {
    Texture2D texture;
    stbi_set_flip_vertically_on_load(flip_vertically);

    int width, height, channel;
    stbi_uc* image = stbi_load(file, &width, &height, &channel, 0);
    if (image) {
        GLenum internal_format(-1);
        GLenum image_format(-1);

        switch (channel) {
            case 1:
                internal_format = GL_R8;
                image_format = GL_RED;
                break;
            case 3:
                internal_format = GL_RGB8;
                image_format = GL_RGB;
                break;
            case 4:
                internal_format = GL_RGBA8;
                image_format = GL_RGBA;
                break;
            default:
                std::cout << "ERROR::TEXTURE2D: The images file format not supported yet!" << std::endl;
        }

        texture.InternalFormat = internal_format;
        texture.ImageFormat = image_format;
        texture.Generate(width, height, image);
    } else {
        std::cout << "ERROR::TEXTURE2D: Failed to load image." << std::endl;
    }
    stbi_image_free(image);
    return texture;
}