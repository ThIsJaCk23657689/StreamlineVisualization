#include <imgui.h>
#include "Game.h"
#include "ResourceManager.h"
#include "SpriteRenderer.h"
#include "Streamline.h"
#include "PostProcessor.h"

SpriteRenderer *Renderer;
Streamline *Stream;
PostProcessor *Effects;

GLboolean IsStreamLineDraw = GL_FALSE;
std::vector<std::string> file_names = {
        "1.vec", "2.vec", "3.vec", "4.vec", "5.vec", "6.vec", "7.vec", "8.vec", "9.vec", "10.vec",
        "11.vec", "12.vec", "13.vec", "14.vec", "15.vec", "16.vec", "19.vec", "20.vec",
        "21.vec", "22.vec", "23.vec", "rect1.vec", "rect2.vec", "test_unit.vec", "test_not_unit.vec", "step5_velocity.vec"
};
std::string current_item = "none";


Game::Game(GLuint width, GLuint height)
    : State(GAME_ACTIVE), Width(width), Height(height) {}

Game::~Game() {
    delete Renderer;
    delete Effects;
}

void Game::Init() {
    ResourceManager::LoadShader("assets/shaders/sprite.vert", "assets/shaders/sprite.frag", nullptr, "sprite");
    ResourceManager::LoadShader("assets/shaders/post_processing.vert", "assets/shaders/post_processing.frag", nullptr, "post_processing");

    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::ortho(-5.0f, 75.0f, -5.0f, 75.0f, -1.0f, 1.0f);
    ResourceManager::GetShader("sprite").Use().SetInt("sprite", 0);
    ResourceManager::GetShader("sprite").SetMat4("projection", projection);

    Renderer = new SpriteRenderer(ResourceManager::GetShader("sprite"));
    Effects = new PostProcessor(ResourceManager::GetShader("post_processing"), this->Width, this->Height);

    // Effects->Shake = GL_TRUE;
    // Effects->Chaos = GL_TRUE;
    // Effects->Confuse = GL_TRUE;
}

void Game::Update(GLfloat dt) {

}

void Game::ProcessInput(GLFWwindow *window, GLfloat dt) {

}

void Game::Render() {
    if (this->State == GAME_ACTIVE) {
        Effects->BeginRender();
        {
            if (IsStreamLineDraw) {
                Stream->Draw();
            }
        }
        Effects->EndRender();
        Effects->Render(glfwGetTime());
    }
}

void Game::RenderGUI() {
    ImGui::Begin("Control Plane");
    ImGuiTabBarFlags tab_bar_flags = ImGuiBackendFlags_None;
    if (ImGui::BeginTabBar("MyTabBar", tab_bar_flags)) {
        if (ImGui::BeginTabItem("General Setting")) {
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::TextColored(ImVec4(0.9f, 0.8f, 0.0f, 1.0f), "Select a vector data to load in.");
            if (ImGui::BeginCombo("Vector Data: ", current_item.c_str())) {
                for (int n = 0; n < file_names.size(); n++) {
                    bool is_selected = (current_item == file_names[n]);
                    if (ImGui::Selectable(file_names[n].c_str(), is_selected)) {
                        current_item = file_names[n];
                    }
                    if (is_selected) {
                        ImGui::SetItemDefaultFocus();
                    }
                }
                ImGui::EndCombo();
            }
            if (ImGui::Button("Load")) {
                Stream = new Streamline(ResourceManager::GetShader("sprite"), 70);
                Stream->Load("assets/data/" + current_item);
                IsStreamLineDraw = GL_TRUE;
            }
            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::End();
}