#include "Streamline.h"
#include <fstream>
#include <iostream>

Streamline::Streamline(Shader &shader, const GLuint sample_density) : SampleDensity(sample_density) {
    this->RenderShader = shader;
}

Streamline::~Streamline() {
    glDeleteVertexArrays(1, &this->VAO);
    glDeleteBuffers(1, &this->VBO);
}

void Streamline::Load(const std::string& filepath) {
    std::ifstream file;
    file.open(filepath, std::ios::in);
    if (file.fail()) {
        file.close();
        std::cout << "ERROR:: Failed to read the vector file at: " << filepath << std::endl;
    }

    std::string x_str, y_str;
    file >> x_str;
    file >> y_str;
    this->Resolution.x = std::stof(x_str);
    this->Resolution.y = std::stof(y_str);
    this->RawData.clear();

    while (file >> x_str && file >> y_str) {
        this->RawData.push_back(glm::vec2(std::stof(x_str), std::stof(y_str)));
    }
    file.close();

    std::cout << "The resolution of data: (" << this->Resolution.x  << ", " << this->Resolution.y << ")" << std::endl;
    std::cout << "The size of raw data: " << this->RawData.size() << std::endl;

    this->Init();
    this->BindBufferData();
}

void Streamline::Draw() {
    /*
    for (auto trail : this->Trails) {
        GLfloat current_size = 1.0f;
        for (auto position : trail) {
            renderer->DrawPoint(position, current_size, glm::vec3(0.9f));
            if (current_size <= 3.0f) {
                current_size += 0.2f;
            }
        }
    }
    */
    this->RenderShader.Use();
    glm::mat4 model = glm::mat4(1.0f);

    this->RenderShader.SetMat4("model", model);
    this->RenderShader.SetVec3("point_color", glm::vec3(1.0f));
    // this->RenderShader.SetFloat("point_size", size);

    glEnable(GL_PROGRAM_POINT_SIZE);

    glBindVertexArray(this->VAO);
    glDrawArrays(GL_POINTS, 0, this->vertices.size() / 4);
    glBindVertexArray(0);
}

void Streamline::Init() {

    this->IsDraw.clear();
    this->IsDraw = std::vector<GLboolean>(this->SampleDensity * this->SampleDensity, GL_FALSE);

    GLuint trail_count = 0;
    GLuint point_index = 0;

    // generating initial position
    GLfloat sample_ticks_x = static_cast<GLfloat>(this->Resolution.x) / this->SampleDensity;
    GLfloat sample_ticks_y = static_cast<GLfloat>(this->Resolution.y) / this->SampleDensity;
    for (GLuint j = 1; j < this->SampleDensity; j++) {
        for (GLuint i = 1; i < this->SampleDensity; i++) {

            // 1. this is the start point of trail
            point_index = 0;
            glm::vec2 current_pos = glm::vec2(i * sample_ticks_x, j * sample_ticks_y);
            GLuint init_point_cell_index = floor(current_pos.y) * this->Resolution.x + floor(current_pos.x);
            // std::cout << "sample cell: (" << i << ", " << j << "), location: (" << current_pos.x << ", " << current_pos.y << "), cell index: " << init_point_cell_index << std::endl;

            // 2. if the cell has a point, don't draw;
            if (this->IsDraw[init_point_cell_index]) {
                continue;
            }

            // 3. if the cell is clear, now we can start draw.
            this->vertices.push_back(current_pos.x);
            this->vertices.push_back(current_pos.y);
            this->vertices.push_back(trail_count);
            this->vertices.push_back(point_index);
            point_index++;
            this->MarkWhichCellIsDraw(current_pos);

            GLuint index = 0;
            do{
                glm::vec2 new_pos = this->MK2(current_pos, 0.2f);
                // std::cout << "r0: (" << current_pos.x << ", " << current_pos.y << "), rh = (" << new_pos.x << ", " << new_pos.y << ")" << std::endl;
                if (new_pos.x > this->Resolution.x || new_pos.y > this->Resolution.y || new_pos.x < 0 || new_pos.y < 0) {
                    // out of the data bonadry
                    trail_count++;
                    break;
                } else {
                    GLuint current_point_cell_index = floor(new_pos.y) * this->Resolution.x + floor(new_pos.x);
                    // std::cout << "new point cell index: " << current_point_cell_index << std::endl;
                    if (this->IsDraw[current_point_cell_index] && current_point_cell_index != init_point_cell_index) {
                        // this cell is draw.
                        // this->Trails.push_back(trail);
                        if (point_index <= 15) {
                            GLuint deleted_index = 0;
                            while (deleted_index < point_index) {
                                GLuint last_index = this->vertices.size() - 1;
                                glm::vec2 last_position = glm::vec2(this->vertices[last_index - 3], this->vertices[last_index - 2]);
                                GLuint delete_point_cell_index = floor(last_position.y) * this->Resolution.x + floor(last_position.x);
                                this->vertices.pop_back();
                                this->vertices.pop_back();
                                this->vertices.pop_back();
                                this->vertices.pop_back();
                                this->IsDraw[delete_point_cell_index] = GL_FALSE;
                                deleted_index++;
                            }
                        }
                        trail_count++;
                        break;
                    }
                    // trail.push_back(new_pos);
                    this->vertices.push_back(new_pos.x);
                    this->vertices.push_back(new_pos.y);
                    this->vertices.push_back(trail_count);
                    this->vertices.push_back(point_index);
                    point_index++;
                    this->MarkWhichCellIsDraw(new_pos);
                    current_pos = new_pos;
                    init_point_cell_index = current_point_cell_index;
                }
                index++;
            } while (index < 10000);

            index = 0;
            do{
                glm::vec2 new_pos = this->MK2(current_pos, -0.2f);
                // std::cout << "r0: (" << current_pos.x << ", " << current_pos.y << "), rh = (" << new_pos.x << ", " << new_pos.y << ")" << std::endl;
                if (new_pos.x > this->Resolution.x || new_pos.y > this->Resolution.y || new_pos.x < 0 || new_pos.y < 0) {
                    // out of the data bonadry
                    trail_count++;
                    break;
                } else {
                    GLuint current_point_cell_index = floor(new_pos.y) * this->Resolution.x + floor(new_pos.x);
                    // std::cout << "new point cell index: " << current_point_cell_index << std::endl;
                    if (this->IsDraw[current_point_cell_index] && current_point_cell_index != init_point_cell_index) {
                        // this cell is draw.
                        // this->Trails.push_back(trail);
                        if (point_index <= 15) {
                            GLuint deleted_index = 0;
                            while (deleted_index < point_index) {
                                GLuint last_index = this->vertices.size() - 1;
                                glm::vec2 last_position = glm::vec2(this->vertices[last_index - 3], this->vertices[last_index - 2]);
                                GLuint delete_point_cell_index = floor(last_position.y) * this->Resolution.x + floor(last_position.x);
                                this->vertices.pop_back();
                                this->vertices.pop_back();
                                this->vertices.pop_back();
                                this->vertices.pop_back();
                                this->IsDraw[delete_point_cell_index] = GL_FALSE;
                                deleted_index++;
                            }
                        }
                        trail_count++;
                        break;
                    }
                    // trail.push_back(new_pos);
                    this->vertices.push_back(new_pos.x);
                    this->vertices.push_back(new_pos.y);
                    this->vertices.push_back(trail_count);
                    this->vertices.push_back(point_index);
                    point_index++;
                    this->MarkWhichCellIsDraw(new_pos);
                    current_pos = new_pos;
                    init_point_cell_index = current_point_cell_index;
                }
                index++;
            } while (index < 10000);

            // this->Trails.push_back(trail);
            trail_count++;
        }
    }
    // std::cout << this->Trails.size() << std::endl;

    std::cout << "Trails amount: " << trail_count << std::endl;
    std::cout << "Points amount: " << this->vertices.size() / 4 << std::endl;
}

void Streamline::BindBufferData() {
    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &this->VBO);
    glBindVertexArray(this->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

glm::vec2 Streamline::MK2(const glm::vec2& p0, const GLfloat h) {
    // Compute the k1 at p0
    glm::vec2 k1 = this->BilinearInterpolation(p0);
    // Compute the intermediate position P1
    glm::vec2 p1 = p0 + h * glm::normalize(k1);
    glm::vec2 k2 = this->BilinearInterpolation(p1);
    // Compute the new sample point
    glm::vec2 new_point = p0 + h * 0.5f * (glm::normalize(k1) + glm::normalize(k2));

    return new_point;
}

glm::vec2 Streamline::BilinearInterpolation(const glm::vec2& position) {
    GLint x1, x2, y1, y2;
    x1 = floor(position.x) > this->Resolution.x - 1 ? this->Resolution.x - 1 : floor(position.x);
    x2 = ceil(position.x) > this->Resolution.x - 1 ? this->Resolution.x - 1: ceil(position.x);
    y1 = floor(position.y) > this->Resolution.y - 1 ? this->Resolution.y - 1 : floor(position.y);
    y2 = ceil(position.y) > this->Resolution.y - 1 ? this->Resolution.y - 1 : ceil(position.y);

    GLfloat dx1 = position.x - x1;
    GLfloat dx2 = 1 - dx1;
    GLfloat dy1 = position.y - y1;
    GLfloat dy2 = 1 - dy1;

    glm::vec2 Q11, Q12, Q21, Q22;
    Q11 = this->RawData[x1 * this->Resolution.y + y1];
    Q12 = this->RawData[x2 * this->Resolution.y + y1];
    Q21 = this->RawData[x1 * this->Resolution.y + y2];
    Q22 = this->RawData[x2 * this->Resolution.y + y2];

    glm::vec2 result_X1 = Q11 * dx2 + Q12 * dx1;
    glm::vec2 result_X2 = Q21 * dx2 + Q22 * dx1;
    glm::vec2 result = result_X1 * dy2 + result_X2 * dy1;

    return result;
}

void Streamline::MarkWhichCellIsDraw(const glm::vec2& position) {
    GLuint x, y;
    x = floor(position.x);
    y = floor(position.y);
    this->IsDraw[y * this->Resolution.x + x] = GL_TRUE;
}