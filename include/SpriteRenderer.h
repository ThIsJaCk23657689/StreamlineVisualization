#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Shader.h"

class SpriteRenderer {
public:
    SpriteRenderer(Shader &shader);
    ~SpriteRenderer();

    void DrawPoint(glm::vec2 position, GLfloat size = 1.0f, glm::vec3 color = glm::vec3(1.0f));

private:
    Shader RenderShader;
    GLuint VAO;

    void InitRenderData();
};