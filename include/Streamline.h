#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>
#include <string>
#include "Shader.h"

const GLfloat MK2_PARAMETER_H = 0.2f;
const GLfloat PATH_LENGTH = 80.0f;
const GLfloat INITIAL_POINT_SIZE = 1.0f;

class Streamline {
public:
    glm::vec3 Resolution;
    std::vector<glm::dvec2> RawData;
    GLuint SampleDensity;
    std::vector<std::vector<glm::vec2>> Trails;
    std::vector<GLfloat> vertices;
    std::vector<GLboolean> IsDraw;

    Streamline(Shader &shader, const GLuint sample_density = 70);
    ~Streamline();
    void Load(const std::string& filepath);
    void Draw();

private:
    GLuint VAO, VBO;
    Shader RenderShader;

    void Init();
    void BindBufferData();
    glm::vec2 MK2(const glm::vec2& r0, const GLfloat h = 0.1f);
    glm::vec2 BilinearInterpolation(const glm::vec2& position);
    void MarkWhichCellIsDraw(const glm::vec2& position);
};