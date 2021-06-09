#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>
#include <string>
#include "SpriteRenderer.h"

class Streamline {
public:
    glm::vec3 Resolution;
    std::vector<glm::vec2> RawData;
    GLuint SampleDensity;
    std::vector<std::vector<glm::vec2>> Trails;
    std::vector<GLboolean> IsDraw;

    Streamline(const GLuint sample_density = 70);
    void Load(const std::string& filepath);
    void Draw(SpriteRenderer *renderer);

private:
    void Init();
    glm::vec2 MK2(const glm::vec2& r0, const GLfloat h = 0.1f);
    glm::vec2 BilinearInterpolation(const glm::vec2& position);
    void MarkWhichCellIsDraw(const glm::vec2& position);
};