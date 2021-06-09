#include "Shader.h"
#include <iostream>

Shader &Shader::Use() {
    glUseProgram(this->ID);
    return *this;
}

void Shader::Compile(const GLchar *vertexSource, const GLchar *fragmentShader, const GLchar *geometrySource) {
    GLuint sVertex, sFragment, sGeometry;

    sVertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(sVertex, 1, &vertexSource, NULL);
    glCompileShader(sVertex);
    this->CheckCompileErrors(sVertex, "VERTEX");

    sFragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(sFragment, 1, &fragmentShader, NULL);
    glCompileShader(sFragment);
    this->CheckCompileErrors(sFragment, "FRAGMENT");

    if (geometrySource != nullptr) {
        sGeometry = glCreateShader(GL_GEOMETRY_SHADER);
        glShaderSource(sGeometry, 1, &geometrySource, NULL);
        glCompileShader(sGeometry);
        this->CheckCompileErrors(sGeometry, "GEOMETRY");
    }

    this->ID = glCreateProgram();
    glAttachShader(this->ID, sVertex);
    glAttachShader(this->ID, sFragment);
    if (geometrySource != nullptr) {
        glAttachShader(this->ID, sGeometry);
    }
    glLinkProgram(this->ID);
    this->CheckCompileErrors(this->ID, "PROGRAM");

    glDeleteShader(sVertex);
    glDeleteShader(sFragment);
    if (geometrySource != nullptr) {
        glDeleteShader(sGeometry);
    }
}

void Shader::SetFloat(const GLchar *name, GLfloat value, GLboolean useShader) {
    if (useShader) {
        this->Use();
    }
    glUniform1f(glGetUniformLocation(this->ID, name), value);
}

void Shader::SetInt(const GLchar *name, GLint value, GLboolean useShader) {
    if (useShader) {
        this->Use();
    }
    glUniform1i(glGetUniformLocation(this->ID, name), value);
}

void Shader::SetBool(const GLchar *name, GLboolean value, GLboolean useShader) {
    if (useShader) {
        this->Use();
    }
    glUniform1i(glGetUniformLocation(this->ID, name), value);
}

void Shader::SetVec2(const GLchar *name, GLfloat x, GLfloat y, GLboolean useShader) {
    if (useShader) {
        this->Use();
    }
    glUniform2f(glGetUniformLocation(this->ID, name), x, y);
}

void Shader::SetVec2(const GLchar *name, const glm::vec2 &vector, GLboolean useShader) {
    if (useShader) {
        this->Use();
    }
    glUniform2fv(glGetUniformLocation(this->ID, name), 1, glm::value_ptr(vector));
}

void Shader::SetVec3(const GLchar *name, GLfloat x, GLfloat y, GLfloat z, GLboolean useShader) {
    if (useShader) {
        this->Use();
    }
    glUniform3f(glGetUniformLocation(this->ID, name), x, y, z);
}

void Shader::SetVec3(const GLchar *name, const glm::vec3 &vector, GLboolean useShader) {
    if (useShader) {
        this->Use();
    }
    glUniform3fv(glGetUniformLocation(this->ID, name), 1, glm::value_ptr(vector));
}

void Shader::SetVec4(const GLchar *name, GLfloat x, GLfloat y, GLfloat z, GLfloat w, GLboolean useShader) {
    if (useShader) {
        this->Use();
    }
    glUniform4f(glGetUniformLocation(this->ID, name), x, y, z, w);
}

void Shader::SetVec4(const GLchar *name, const glm::vec4 &vector, GLboolean useShader) {
    if (useShader) {
        this->Use();
    }
    glUniform4fv(glGetUniformLocation(this->ID, name), 1, glm::value_ptr(vector));
}

void Shader::SetMat4(const GLchar *name, const glm::mat4 &matrix, GLboolean useShader) {
    if (useShader) {
        this->Use();
    }
    glUniformMatrix4fv(glGetUniformLocation(this->ID, name), 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::CheckCompileErrors(GLuint object, std::string type) {
    GLint success;
    GLchar infoLog[1024];
    if (type != "PROGRAM") {
        glGetShaderiv(object, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(object, 1024, NULL, infoLog);
            std::cout << "| ERROR::SHADER: Compile-time error: Type: " << type << "\n"
                << infoLog << "\n -- ---------------------------------------- --"
                << std::endl;
        }
    } else {
        glGetProgramiv(object, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(object, 1024, NULL, infoLog);
            std::cout << "| ERROR::SHADER: Link-time error: Type: " << type << "\n"
                      << infoLog << "\n -- ---------------------------------------- --"
                      << std::endl;
        }
    }
}