#pragma once

#include <glad/glad.h>

class Texture2D {
public:
    GLuint ID;
    GLuint Width, Height;
    GLuint InternalFormat;
    GLuint ImageFormat;
    GLuint WrapS;
    GLuint WrapT;
    GLuint FilterMin;
    GLuint FilterMax;

    Texture2D();
    void Generate(GLuint width, GLuint height, unsigned char* data);
    void Bind() const;
};