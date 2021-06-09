#include "SpriteRenderer.h"
#include <iostream>

SpriteRenderer::SpriteRenderer(Shader &shader) {
    this->RenderShader = shader;
    this->InitRenderData();
}

SpriteRenderer::~SpriteRenderer() {
    glDeleteVertexArrays(1, &this->VAO);
}

void SpriteRenderer::DrawPoint(glm::vec2 position, GLfloat size, glm::vec3 color) {
    this->RenderShader.Use();
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(position, 0.0f));

    this->RenderShader.SetMat4("model", model);
    this->RenderShader.SetVec3("sprite_color", color);
    this->RenderShader.SetFloat("point_size", size);

    glEnable(GL_PROGRAM_POINT_SIZE);

    glBindVertexArray(this->VAO);
    glDrawArrays(GL_POINTS, 0, 1);
    glBindVertexArray(0);
}

void SpriteRenderer::InitRenderData() {
    GLuint VBO;
    GLfloat vertices[] = {
            0.0f, 0.0f,
    };

    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(this->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}


