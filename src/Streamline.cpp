#include "Streamline.h"
#include <fstream>
#include <iostream>

Streamline::Streamline(const GLuint sample_density) : SampleDensity(sample_density) {

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

    while (file >> x_str && file >> y_str) {
        this->RawData.push_back(glm::vec2(std::stof(x_str), std::stof(y_str)));
    }
    file.close();

    std::cout << "The resolution of data: (" << this->Resolution.x  << ", " << this->Resolution.y << ")" << std::endl;
    std::cout << "The size of raw data: " << this->RawData.size() << std::endl;

    this->Init();
}

void Streamline::Draw(SpriteRenderer *renderer) {
    for (auto trail : this->Trails) {
        GLfloat current_size = 1.0f;
        for (auto position : trail) {
            renderer->DrawPoint(position, current_size, glm::vec3(0.1f));
            if (current_size <= 3.0f) {
                current_size += 0.2f;
            }
        }
    }
}

void Streamline::Init() {

    this->IsDraw.clear();
    this->IsDraw = std::vector<GLboolean>(this->SampleDensity * this->SampleDensity, GL_FALSE);

    // generating initial position
    GLfloat sample_ticks_x = static_cast<GLfloat>(this->Resolution.x) / this->SampleDensity;
    GLfloat sample_ticks_y = static_cast<GLfloat>(this->Resolution.y) / this->SampleDensity;
    for (GLfloat i = 1; i < this->SampleDensity; i++) {
        for (GLfloat j = 1; j < this->SampleDensity; j++) {
            std::vector<glm::vec2> trail;
            glm::vec2 current_pos = glm::vec2(j * sample_ticks_x, i * sample_ticks_y);
            if (this->IsDraw[current_pos.y * this->Resolution.x + current_pos.x]) {
                continue;
            }
            trail.push_back(current_pos);
            this->MarkWhichCellIsDraw(current_pos);

            GLuint i = 0;
            do{
                glm::vec2 new_pos = this->MK2(current_pos, 0.2f);
                // std::cout << "r0: (" << current_pos.x << ", " << current_pos.y << "), rh = (" << new_pos.x << ", " << new_pos.y << ")" << std::endl;
                if (new_pos.x > this->Resolution.x && new_pos.y > this->Resolution.y || new_pos.x < 0 && new_pos.y < 0) {
                    break;
                } else {
                    if (this->IsDraw[new_pos.y * this->Resolution.x + new_pos.x]) {
                        break;
                    }
                    trail.push_back(new_pos);
                    this->MarkWhichCellIsDraw(new_pos);
                    current_pos = new_pos;
                }
                this->Trails.push_back(trail);
                i += 1;
            } while (i < 80);

            i = 0;
            do{
                glm::vec2 new_pos = this->MK2(current_pos, -0.2f);
                // std::cout << "r0: (" << current_pos.x << ", " << current_pos.y << "), rh = (" << new_pos.x << ", " << new_pos.y << ")" << std::endl;
                if (new_pos.x > this->Resolution.x && new_pos.y > this->Resolution.y || new_pos.x < 0 && new_pos.y < 0) {
                    break;
                } else {
                    if (this->IsDraw[new_pos.y * this->Resolution.x + new_pos.x]) {
                        break;
                    }
                    trail.push_back(new_pos);
                    this->MarkWhichCellIsDraw(new_pos);
                    current_pos = new_pos;
                }
                this->Trails.push_back(trail);
                i += 1;
            } while (i < 80);
        }
    }
    std::cout << this->Trails.size() << std::endl;
}

glm::vec2 Streamline::MK2(const glm::vec2& r0, const GLfloat h) {
    // Compute the U0 at r0
    glm::vec2 U_0 = this->BilinearInterpolation(r0);
    // Compute the intermediate position P1
    glm::vec2 p1 = r0 + h * U_0;
    glm::vec2 U_1 = this->BilinearInterpolation(p1);
    // Compute the new sample point r(h)
    glm::vec2 r_h = r0 + (h / 2.0f) * (U_0 + U_1);

    return r_h;
}

glm::vec2 Streamline::BilinearInterpolation(const glm::vec2& position) {
    GLfloat x1, x2, y1, y2;
    // std::cout << position.x << ", " << position.y << std::endl;
    x1 = floor(position.x);
    x2 = x1 + 1;
    y1 = floor(position.y);
    y2 = y1 + 1;

    glm::vec2 Q11, Q12, Q21, Q22;
    Q11 = this->RawData[y1 * this->Resolution.x + x1];
    Q12 = this->RawData[y1 * this->Resolution.x + x2];
    Q21 = this->RawData[y2 * this->Resolution.x + x1];
    Q22 = this->RawData[y2 * this->Resolution.x + x2];

    glm::vec2 result_X1 = (x2 - position.x) * Q11 + (position.x - x1) * Q21;
    glm::vec2 result_X2 = (x2 - position.x) * Q12 + (position.x - x1) * Q22;
    glm::vec2 result = (y2 - position.y) * result_X1 + (position.y - y1) * result_X2;

    // std::cout << result.x << ", " << result.y << std::endl;
    return result;
}

void Streamline::MarkWhichCellIsDraw(const glm::vec2& position) {
    GLfloat x, y;
    x = floor(position.x);
    y = floor(position.y);
    this->IsDraw[y * this->Resolution.x + x] = GL_TRUE;
}