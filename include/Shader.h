#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>

class Shader {
public:
    GLuint ID;
    Shader() {}
    Shader &Use();
    void Compile(const GLchar *vertexSource, const GLchar *fragmentShader, const GLchar *geometrySource = nullptr);

    void SetFloat(const GLchar *name, GLfloat value, GLboolean useShader = false);
    void SetInt(const GLchar *name, GLint value, GLboolean useShader = false);
    void SetBool(const GLchar *name, GLboolean value, GLboolean useShader = false);
    void SetVec2(const GLchar *name, GLfloat x, GLfloat y, GLboolean useShader = false);
    void SetVec2(const GLchar *name, const glm::vec2 &vector, GLboolean useShader = false);
    void SetVec3(const GLchar *name, GLfloat x, GLfloat y, GLfloat z, GLboolean useShader = false);
    void SetVec3(const GLchar *name, const glm::vec3 &vector, GLboolean useShader = false);
    void SetVec4(const GLchar *name, GLfloat x, GLfloat y, GLfloat z, GLfloat w, GLboolean useShader = false);
    void SetVec4(const GLchar *name, const glm::vec4 &vector, GLboolean useShader = false);
    void SetMat4(const GLchar *name, const glm::mat4 &matrix, GLboolean useShader = false);

private:
    void CheckCompileErrors(GLuint object, std::string type);
};
